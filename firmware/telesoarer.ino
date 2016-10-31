//#define DEBUG 1

#include "telesoarer.h"
#include "ms5611.h"
#include <EEPROM.h>
#include <avr/wdt.h>

void storeRunMode(unsigned char newmode)
{
  EEPROM.update(0, newmode);
}

void beeperOn()
{
  LED_ON(LED1);
  // PWM ON
  TCCR0A |= (0b10<<COM0B0);
}

void beeperOff()
{
  LED_OFF(LED1);
  TCCR0A &= ~(0b11<<COM0B0);
}

void morseStart()
{
  if (morseState != STOP) return;
  morseState = START;
}

void morseStop()
{
  morseState = STOP;
  beeperOff();
}

char *morseString;
static inline void morseStateMachine() // called regularly from ISR
{
  static unsigned int timer;
  static unsigned char bitCount;
  static unsigned char letterBits;
  static unsigned char charIndex;
  static char thisChar;

  switch (morseState)
  {
    case DIT:
      if (timer == 0)
        beeperOn();
      if (++timer > DITLENGTH) 
      {
        timer = 0;
        morseState = BITGAP;
      }
      break;

    case DAH:
      if (timer == 0)
        beeperOn();
      if (++timer > DITLENGTH * 3)
      {
        timer = 0;
        morseState = BITGAP;
      }
      break;

    case BITGAP:
      if (timer == 0) 
        beeperOff();
      if (++timer > DITLENGTH) morseState = NEXTBIT;
      break;

    case LETTERGAP:
      if (timer == 0) 
        beeperOff();
      if (++timer > DITLENGTH * 2) morseState = NEXTCHAR;
      break;

    case SPACE:
      if (timer == 0) 
        beeperOff();
      if (++timer > DITLENGTH * 6) morseState = NEXTCHAR;
      break;

    case NEXTBIT:

      timer = 0;

      if (letterBits & 0b10000000)
        morseState = DAH;
      else
        morseState = DIT;

      letterBits <<= 1; // shift bits left 1 position
      if (bitCount-- == 0)
        morseState = LETTERGAP;

      break;

    case NEXTCHAR:

      thisChar = pgm_read_byte(&morseString[charIndex++]);

      if (thisChar == ' ') 
      {
        timer = 0;
        morseState = SPACE;
      }
      else if ( (thisChar == '\0') || (thisChar < ',') || (thisChar > 'Z') ) // end of string or invalid character?  
        morseState = STOP;
      else
      {
        letterBits = pgm_read_byte(&morseTable[thisChar-44]);
        bitCount = (letterBits >> 5);
        if (bitCount < 6)
          letterBits <<= 3;
        else
          letterBits <<= 2;

        morseState = NEXTBIT;
      }
      break;


    case START:
      beeperOff();
      timer = 0;
      charIndex = 0;
      morseState = NEXTCHAR;
      break;

    case STOP:
      break; // do nothing

    default:
      morseState = STOP;
      break;

  } // switch
      
}

#define PULSE_INVALID (-1)
volatile int _sigPulseWidth = PULSE_INVALID;
volatile unsigned int pulseTimeoutCounter = 0;
volatile bool runningIndicatorEnabled = false;
//ISR(TIMER2_COMPA_vect) // ISR called every 1ms
ISR(TIMER0_COMPA_vect) // ISR is called every 1/PIEZO_FREQ seconds (usually 1/2.8KHz)
{  
  static uint8_t div4 = 0;
  if (div4++ % 3) return;
  
  if (pulseTimeoutCounter++ > PULSE_TIMEOUT)
  {
    _sigPulseWidth = PULSE_INVALID;  // signal no pulse detected
    pulseTimeoutCounter--; // prevent counter incrementing further
  }

  // blink middle (blue) LED briefly, each three seconds to indicate RUNNING state (if so)
  static unsigned int runningIndicatorCount = 0;
  if (runningIndicatorEnabled)
  {
    if (runningIndicatorCount++ == 2999) runningIndicatorCount = 0;
    if (runningIndicatorCount == 0) {
      LED_ON(LED2);
    }
    else if (runningIndicatorCount == 120) 
    {
      LED_OFF(LED2);  // LED on for 120ms
    }
  }
  else LED_OFF(2);

  morseStateMachine();  // iterate the Morse state machine (1ms interval) 
}

// Input capture on ICP1 pin (PB0/PCINT0 -- Arduino Pin 8)
ISR(TIMER1_CAPT_vect)
{
  // an edge detection event has occured on the ICP1(PB0) pin
  unsigned int icr;
  TCNT1 = 0;         // reset counter
  
  icr = (ICR1 >> 1); // capture and convert 2MHz counter to number of milliseconds

  if (TCCR1B & (1<<ICES1)) // trigger was high going edge
  {
    TCCR1B &= ~(1<<ICES1); // set next trigger to low going edge and do nothing more
  }
  else // trigger was low going edge
  {
    if ((icr >= 800) && (icr < 2200)) 
    {
      pulseTimeoutCounter = 0;
      _sigPulseWidth = icr;
    }
    else
    {
      _sigPulseWidth = PULSE_INVALID; // invalid pulse width
    }
    TCCR1B |= (1<<ICES1); // set next trigger to high going edge
  }

}

void setup() {
#ifdef DEBUG
  Serial.begin(115200);
  Serial.println(F("START"));
#endif

  // LED output pins
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(PIEZO_PIN, OUTPUT);
  pinMode(SIG_PIN, INPUT_PULLUP);
  pinMode(ALT_PIN, OUTPUT);
  
  // all LEDs test blink commence
  LED_ON(LED1);
  LED_ON(LED2);
  LED_ON(LED3);  
  
  _delay_ms(100); // allow time for MS5611 to power up and stabalise
  MS5611_init();
  
  // all LEDs test blink end
  LED_OFF(LED1);
  LED_OFF(LED2);
  LED_OFF(LED3);
}

enum { INIT = 0, WAIT_READY, READY, RUNNING, PROGRAM } runState = INIT;
enum runMode_t { NORMAL = 0, INACTIVITY } runMode = NORMAL;

void loop() {

  double TEMP = 0;

  static uint64_t LPRESS = 0; 
  if (!LPRESS) LPRESS = getPressure();
  
  uint32_t PRESS = (LPRESS * 7 + getPressure()) / 8; // low pass filter
  
#ifdef DEBUG
  TEMP = getLastTemp();
  Serial.print(F("TEMP=")); Serial.print(TEMP,2); Serial.print(F("ºC"));
  Serial.print(F("   "));
  Serial.print(F("PRESS=")); Serial.print(PRESS);
  Serial.print(F("   "));
  Serial.print(F("DELTA=")); Serial.print((int)(PRESS-LPRESS));
  Serial.println();
#endif

  OCR2B = map( constrain((int)(PRESS-LPRESS), -126, 126), -126, 126, 254, 1);
  LPRESS = PRESS;
  
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////

  runningIndicatorEnabled = (runState == RUNNING) ? true : false;
  int sigPulseWidth = _sigPulseWidth;

  /**************************
   ****  STATE MACHINE   ****
   **************************/  
  switch (runState) 
  {
    case INIT: 
    {
      cli();
      wdt_reset();
      wdt_enable(WDTO_4S);  // 4 second watchdog timer. (Brown Out Detector fuses are set for 2.7V)

      // Set up PWM output on OC0B pin for driving the piezo at 2.8KHz
      // NOTE! This overrides Arduino and PWM and delay() internals! Use _delay_ms() instead.)
      TCCR0A = (0b01<<WGM00); // 8-bit Phase Correct PWM mode, output on OC0B (PD5) ...
      TCCR0B = (1<<WGM02) | (0b011<<CS00); // ... clk/64 
      OCR0A = 45; // TOP = 45(see FSM READY: state) / 2 gives ~2.8KHz
      OCR0B = PWM_DUTY_CYCLE_QUIET;

      // also use Timer0 for our internal timing, at 2.8KHz of course, becauset that's the piezo's resonant frequency.
      TIMSK0 = (1<<OCIE0A);
      
      // Timer1 is for input capture compare on ICP1 on PB0/PCINT0 -- Arduino Pin 8
      TCCR1A = 0;               // no waveform generator, run counter in normal mode
      TCCR1B = (0b010<<CS10);   // clk/8 for 2MHz counting (16Mhz main clk)      
      TCCR1B |= (1<<ICES1);     // capture trigger on rising edge, to begin with
      TCNT1 = 0;                // reset the counter
      TIMSK1 = (1<<ICIE1);      // enable input capture interrupt
            
      // Timer2 will be our 8-bit altitude delta PWM output on PD3/OC2B -- Arduino Pin 3
      // WARNING!: Arduino usually uses Timer2. We break that here. DO NOT USE delay(). Use _delay_ms() instead
      TCCR2A = (0b01<<WGM20); // 8-bit Phase Correct PWM mode 1 (TOP=0xFF)
      TCCR2B = (0b100<<CS20); // ... clk/64
 //     OCR2A = 255; // TOP = 255 for about 980Hz using 16MHz clk/64
      OCR2B = 127; // mid-scale to begin.
      TCCR2A |= (0b10<<COM2B0); // connect to OC2B output pin (mode 2)
      
      sei(); // enable all interrupts

      // retrieve stored Run Mode from EEPROM
      runMode = (runMode_t)EEPROM.read(0x00);
      if (runMode > INACTIVITY) // sanitize
      {
        runMode = NORMAL;
        storeRunMode(runMode);
      }

      // CHECK FOR CONFIGURATION MODE ENTRY REQUEST
      _delay_ms(1000);             // allow one second for the receiver to boot up
      wdt_reset();
      unsigned int pulseSum = 0;
      unsigned int lastPW = 0;
      int count;
      for (count = 0; count < 10; count++)
      {
        int thisPulse = _sigPulseWidth;
        if (lastPW == 0) lastPW = thisPulse;
        if (thisPulse > 0)
        {
          pulseSum += abs(thisPulse - lastPW);
          lastPW = thisPulse;
        }
        _delay_ms(50);
      }
      if ((pulseSum / count) > 200) // enter programming mode if average pulse delta large enough 
      {
        runState = PROGRAM;
        break;
      }

      runState = WAIT_READY;
      break;
    }

    case WAIT_READY: 
    {
      LED_ON(LED3);

      if ((sigPulseWidth == PULSE_INVALID) && (morseState == STOP))
      {
        morseString = (char *)PSTR("E   ");
        morseStart();
      }
      else if (morseState == STOP)
      {
        LED_OFF(LED3);

        if (runMode == NORMAL)
          morseString = (char *)PSTR("R N ");
        else
          morseString = (char *)PSTR("R I ");
        morseStart();
        runState = READY;
      }
      break;
    }

    case READY:
    {
      if (morseState == STOP)
      {
        // Set beeper frqeuency to ~2.8KHz at 15% duty cycle = loudest according to tests.
        OCR0A = 45;
        OCR0B = PWM_DUTY_CYCLE;
        
        // emit a single, loud "BIP!"
        if (sigPulseWidth < MIDPOINT)
        {
          beeperOn();
          _delay_ms(150);
          beeperOff();
        }

        runState = RUNNING;
      }
      break;
    }

    case RUNNING: 
    {
      switch (runMode)
      {
        case NORMAL: /* manually switched at transmitter */
        {
          if (sigPulseWidth > 0)
          {
            if (sigPulseWidth > MIDPOINT)
            {
              morseString = (char *)PSTR("LOST "); 
              morseStart();    // start bleeting loud morse tones
            }
            else
              morseStop();
          } 
          else
          {
            morseStop();
            runState = WAIT_READY; // lost the pulse signal
          }
        
          break;
        }

        case INACTIVITY: /* enteres lost mode if no acitivity seen for a while */
        {
          static int last_sigPulseWidth = 0;
          static int last_sigPulseWidthAverage = 0;
          static int sigPulseWidthAverage;
          static unsigned int inactiveTimer = 0;
          
          if (sigPulseWidth > 0) // if pulse has not timed out
          {
            int thisPulse = sigPulseWidth;
            if (last_sigPulseWidth == 0) last_sigPulseWidth = thisPulse;
            sigPulseWidthAverage = ((last_sigPulseWidth * 7) + thisPulse) / 8; // some noise filtering
            last_sigPulseWidth = thisPulse;

            if (last_sigPulseWidthAverage == 0)
              last_sigPulseWidthAverage = sigPulseWidthAverage;

            if ( (sigPulseWidthAverage > (last_sigPulseWidthAverage-20)) 
                && (sigPulseWidthAverage < (last_sigPulseWidthAverage+20/*TODO sensitivity*/)) )
            {
              if (inactiveTimer++ > 600/*TODO -- how long to wait for inactivity before alarming*/)
              {
                inactiveTimer--; // don't let this keep climbing
                if (morseState == STOP)
                {
                  morseString = (char *)PSTR("LOST "); 
                  morseStart();   // start bleeting loud morse tones
                }
              }
            }
            else
            {
              inactiveTimer = 0;
              morseStop();
            }
            last_sigPulseWidthAverage = sigPulseWidthAverage;

          }
          else
          {
            morseStop();
            runState = WAIT_READY; // we lost the signal entirely. Go quiet.
          }

        }
      }

      break; // case RUNNING
    }

    case PROGRAM: 
    {
      static enum { 
        WAIT, // wait for pulse to go above MIDPOINT before entering program mode
        ENTER, 
        ASK_NORMAL, 
        ANS_NORMAL, 
        ASK_INACTIVE, 
        ANS_INACTIVE, 
        OK, 
        EXIT 
      } pgmState = WAIT;
      static int pgmTimer = 0;


      switch (pgmState)
      {
        case WAIT:
          if (sigPulseWidth < (MIDPOINT * 1.17)) // at least 3/4 stick needed (allowing for first 1ms being included)
          { 
            if (morseState == STOP)
            {
              LED_TOGGLE(LED2);
              morseString = (char *)PSTR("W  ");
              morseStart();
            }
          }
          else if (pgmTimer++ > 100)
          {
            pgmTimer = 0;
            morseStop();
            pgmState = ENTER;
          }
          break;

        case ENTER:
          LED_ON(LED1);
          morseString = (char *)PSTR("P  ");
          morseStart();
          pgmState = ASK_NORMAL;
          break;

        case ASK_NORMAL:
          if (morseState == STOP) // wait for morse code to finish sounding
          {
            pgmTimer = 0;
            morseString = (char *)PSTR("N? ");
            morseStart();
            pgmState = ANS_NORMAL;
          }
          break;

        case ANS_NORMAL:
          if (morseState == STOP) // wait for morse code to finish sounding
          {
            if (pgmTimer++ < 200) // did they say yes?
            {
              if (sigPulseWidth < MIDPOINT)
              {
                storeRunMode(NORMAL);
                pgmState = OK;
              }
            } else
              pgmState = ASK_INACTIVE;
          }
          break;

        case ASK_INACTIVE:
          if (morseState == STOP) // wait for morse code to finish sounding
          {
            pgmTimer = 0;
            morseString = (char *)PSTR("I? ");
            morseStart();
            pgmState = ANS_INACTIVE;
          }
          break;

        case ANS_INACTIVE:
          if (morseState == STOP) // wait for morse code to finish sounding
          {
            if (pgmTimer++ < 200) // did they say yes?
            {
              if (sigPulseWidth < MIDPOINT)
              {
                storeRunMode(INACTIVITY);
                pgmState = OK;
              }
            } else
              pgmState = ASK_NORMAL;
          }
          break;

        case OK:
          morseString = (char *)PSTR("OK ");
          morseStart();
          pgmState = EXIT;
          break;

        case EXIT:
          if (morseState == STOP) // wait for morse code to finish sounding
            runState = INIT;
          break;
      }

      break;
    }

    default:
      runState = INIT;

  } // FSM's switch() 

  _delay_ms(10); // XXX TODO: fix this ugly hack,  here just to slow down asynch counters for PROGRAM and INACTIVITY modes

   wdt_reset();

}
