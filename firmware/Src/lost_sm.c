#include "lost_sm.h"
#include "morse.h"

Lost_State LOST_STATE = LS_BOOTING;

volatile runModeTypeDef runMode = RM_NORMAL;

void lostModelStateMachine()
{
  /*************************************
   ****  LOST MODEL RUN_STATE MACHINE   ****
   *************************************/
  static enum { RS_INIT = 0, RS_WAIT_READY, RS_ASK_PROG, RS_READY, RS_READY2, RS_RUNNING, RS_PROGRAM } RUN_STATE = RS_INIT;
  static enum { PS_INIT, PS_WAIT, PS_ENTER, PS_ASK_NORMAL, PS_ANS_NORMAL, PS_ASK_INACTIVE, PS_ANS_INACTIVE, PS_OK, PS_EXIT } pgmState = PS_INIT;

  if (RUN_STATE == RS_ASK_PROG || (RUN_STATE == RS_PROGRAM && (pgmState == PS_INIT || pgmState == PS_WAIT)))
  {
    // blink RED LED
    HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, !(HAL_GetTick() & 0x80)); 
  }

  switch (RUN_STATE) 
  {
    case RS_INIT: 
      LOST_STATE = LS_BOOTING;
      HAL_TIM_PWM_Start_IT(&htim3, TIM_CHANNEL_2); // start VARIO PWM output on Timer 3 Ch1 (VARIO on PB0)
      HAL_TIM_IC_Start_IT(&htim4, TIM_CHANNEL_1); // input capture PULSE START (rising) CCR1=PWM_period from Timer 4 Ch1 (SIG on PB6)
      HAL_TIM_IC_Start_IT(&htim4, TIM_CHANNEL_2); // input capture PULSE END (falling) CCR2=pulse_width from Timer 4 Ch2 (SIG on PB6-indirect)

      /* TODO:
      // retrieve stored Run Mode from EEPROM
      runMode = (enum runMode_t)eeprom_read_byte(0x00);
      if (runMode > INACTIVITY) // sanitize
      */
      {
        runMode = RM_NORMAL; // RM_INACTIVITY;
        // TODO:         storeRunMode(runMode);
      }

      if (HAL_GetTick() < 1500) break; // give the RC receiver, display, MS5611 etc plenty of time to boot up
      RUN_STATE = RS_WAIT_READY;
      break;

      // wait for a valid pulsewidth signal
    case RS_WAIT_READY: 
      {
        if (sigPulseWidth < 0)
        {
          if (!activityTimer)
          {
            LOST_STATE = LS_NOSIG;
            activityTimer = 1000;
            morseSend("E");
          }
          HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, ((activityTimer > 850)? 1 : 0));
        }
        else 
        {
          RUN_STATE = RS_ASK_PROG;
        }
        break;
      }

      // give user a chance to enter configuration, "program" mode
    case RS_ASK_PROG:
      {
        LOST_STATE = LS_ASK_PROG;

        if (!isMorseReady()) break;  // give the "E" a chance to be sent

        static uint32_t period_ticks = 0;
        //      printf("PT:%6u\r\n", period_ticks);

        // indicate program entry time window start
        if (!period_ticks) 
        {
          morseSend("?"); // once
          period_ticks = HAL_GetTick() + 3000;
        }
        // Check for continuous SIG input.
        // If there's significant accumlated movement over a one second period, 
        // then the user is requesting programming mode
        else
        {
          static uint32_t pulseSum = 0;
          static int16_t lastPW = 0;
          int16_t thisPW = sigPulseWidth;
          if (lastPW == 0) lastPW = thisPW;
          if (thisPW > 0)  // (pulse width -1 indicates invalid RC receiver signali. ignore.)
          {
            pulseSum += abs(thisPW - lastPW); 
            lastPW = thisPW;
            if (pulseSum > 500) // enter programming mode if there's been 500+ input delta movement
            {
              pgmState = PS_INIT;
              RUN_STATE = RS_PROGRAM;
              LED_ON(RED);
              break;
            }
          }
          // time up yet?
          if (HAL_GetTick() > period_ticks)
          {
            LED_OFF(RED);
            RUN_STATE = RS_READY;
          }
        }
        break;
      }

    case RS_READY:
      {
        // Set beeper frqeuency to ~2.8KHz at 15% duty cycle = LOUD!
        /* TODO:
           OCR0A = 180;
           OCR0B = PWM_DUTY_CYCLE;
           */
        if (runMode == RM_NORMAL)
          morseSend(" AN");
        else
          morseSend(" AI");
        RUN_STATE = RS_READY;

        activityTimer = INACTIVIY_PERIOD;
        RUN_STATE = RS_READY2;
        break;
      }

    case RS_READY2:
      LOST_STATE = (runMode == RM_NORMAL) ? LS_ARMED_NORMAL : LS_ARMED_INACTIVITY;
      if (!isMorseReady()) break;
      RUN_STATE = RS_RUNNING;
      break;

    case RS_RUNNING: 
      switch (runMode)
      {
        case RM_NORMAL: /* manually switched at transmitter */
          if ( (sigPulseWidth > MIDPOINT) || (sigPulseWidth < 0) )
          {
            LOST_STATE = LS_LOST;
            morseSend("LOST "); 
          }
          else
          {
            morseStop();
            RUN_STATE = RS_READY2;
          }
          break;

        case RM_INACTIVITY: /* enters lost mode if no acitivity seen for a while */
          {
            static int last_sigPulseWidth = 0;
            static int last_sigPulseWidthAverage = 0;
            static int sigPulseWidthAverage;

            if (sigPulseWidth > 0) // if pulse has not timed out
            {
              int thisPulse = sigPulseWidth;
              if (last_sigPulseWidth == 0) last_sigPulseWidth = thisPulse;
              sigPulseWidthAverage = ((last_sigPulseWidth * 7) + thisPulse) / 8; // some noise filtering
              last_sigPulseWidth = thisPulse;

              if (last_sigPulseWidthAverage == 0)
                last_sigPulseWidthAverage = sigPulseWidthAverage;

              if ( (sigPulseWidthAverage > (last_sigPulseWidthAverage-20)) 
                  && (sigPulseWidthAverage < (last_sigPulseWidthAverage+20/*TODO: configurable sensitivity */)) )
              {
                if (!activityTimer)
                {
                  LOST_STATE = LS_LOST;
                  morseSend("LOST "); 
                }
              }
              else
              {
                activityTimer = INACTIVIY_PERIOD;
                morseStop();
                RUN_STATE = RS_READY2;
              }
              last_sigPulseWidthAverage = sigPulseWidthAverage;

            }
            else
            {
              morseStop();
              RUN_STATE = RS_WAIT_READY; // we lost the signal entirely. Go quiet.
            }

          }
      } // switch(runMode)
      break; // case RUNNING

    case RS_PROGRAM: 
      {
        switch (pgmState)
        {
          case PS_INIT:
            // give the user 3 seconds to move sigPulseWidth above MIDPOINT and leave it steady -- or abort PROGRAM state 
            activityTimer = 3000;
            pgmState = PS_WAIT;
            break;

          case PS_WAIT:
            {
              LOST_STATE = LS_PROG_WAIT;
              LED_ON(RED);
              if (!auxTimer) // ms down counter
              {
                auxTimer = 100;
                LED_TOGGLE(RED); // 5 flashes a second
                morseSend("W  ");
              }

              // wait to be sure that any sig input chaing has ended ...
              static int lastPulse = 0;
              if (!lastPulse) lastPulse = sigPulseWidth;
              static int silenceCounter = 0;
              if (silenceCounter++ < 500) // half a second silence is required
              {    
                if ( (sigPulseWidth >> 1) != (lastPulse >> 1) )
                {
                  lastPulse = sigPulseWidth; 
                  silenceCounter = 0; // got noise. reset the counter
                }
              }
              else if (sigPulseWidth >  MIDPOINT) pgmState = PS_ENTER;
              else pgmState = PS_EXIT;

              if (!activityTimer) pgmState = PS_EXIT; // time's up. we're outta here

              if (pgmState != PS_WAIT) activityTimer = 1000; // next state entry delay

              break;
            }

          case PS_ENTER:
            if (activityTimer) break; // hold off
            LOST_STATE = LS_PROGRAMMING;
            morseSend("P  "); // we've entered programming mode
            activityTimer = 1000;
            pgmState = PS_ASK_NORMAL;
            break;

          case PS_ASK_NORMAL:
            if (activityTimer) break;
            if (!isMorseReady()) break;
            LOST_STATE = LS_PROG_NORMAL;
            morseSend("N?"); // ask, "Normal mode?"
            activityTimer = 3000;
            pgmState = PS_ANS_NORMAL;
            break;

          case PS_ANS_NORMAL:
            if (sigPulseWidth < MIDPOINT) // user answers "yes" by moving to stick bottom
            {
              /* TODO
                 storeRunMode(NORMAL);
                 */
              pgmState = PS_OK; // we're done
            } 
            else if (!activityTimer)
            {
              pgmState = PS_ASK_INACTIVE;  // timed out, so move to next question
            }
            break;

          case PS_ASK_INACTIVE:
            if (activityTimer) break;
            if (!isMorseReady()) break;
            LOST_STATE = LS_PROG_INACTIVITY;
            morseSend("I?");  // ask, "Inactivity mode?"
            activityTimer = 3000;
            pgmState = PS_ANS_INACTIVE;
            break;

          case PS_ANS_INACTIVE:
            if (sigPulseWidth < MIDPOINT) // user answers "yes" by moving to stick bottom
            {
              /* TODO
                 storeRunMode(INACTIVITY);
                 */
              pgmState = PS_OK; // we're done
            }
            else if (!activityTimer)
            {
              pgmState = PS_ASK_NORMAL; // timed out, so back to first question
            }
            break;

          case PS_OK:
            LOST_STATE = LS_PROG_OK;
            morseSend("OK");
            activityTimer = 1000;
            pgmState = PS_EXIT;
            break;

          case PS_EXIT:
            if (activityTimer) break;
            if (!isMorseReady()) break;
            LED_OFF(RED);
            RUN_STATE = RS_READY;
            break;
        } // switch(pgmState)

        break;
      } // case PROGRAM:

  } // switch(RUN_STATE) LOST MODEL RUN_STATE MACHINE 
}

