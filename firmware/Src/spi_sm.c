#include "spi_sm.h"
#include "display.h"
#include "ms5611.h"

/*********************************************
 * DMA SPI State Machine for OLED and VARIO *
 *********************************************/
// The OLED and VARIO use the same DMA/SPI bus and thus combined into a single state machine.
void spiStateMachine()
{
  static enum {
    OLED_REFRESH,
    VARIO_READ_TEMPERATURE,
    VARIO_READ_PRESSURE
  } STATE = OLED_REFRESH;
//  static uint32_t OLED_ticks = 0; // for non-blocking lengthy delays
//  static uint32_t VARIO_ticks = 0; // for non-blocking lengthy delays

  // We share a single SPI bus between an OLED display and an VARIO variometer sensor.
  // Thus, only one is accessed at a time and both are controlled within this single state machine.
  
  if (!OLED_RESET_DONE || !VARIO_RESET_DONE) return;

  switch (STATE)
  {
    case OLED_REFRESH: // copy internal screen buffer out to OLED
    {
      if (OLED_RESET_DONE)
      {
        static uint8_t phase = 0;
        switch (phase)
        {
          case 0: // Phase 0 of 2 -- Set up and start DMA screen data reresh
            if (HAL_SPI_GetState(&hspi2) != HAL_SPI_STATE_READY) break; // previous DMA transfer completed?
            HAL_GPIO_WritePin(OLED_SS_GPIO_Port, OLED_SS_Pin, 0); // assert OLED_SS
            HAL_GPIO_WritePin(OLED_DC_GPIO_Port, OLED_DC_Pin, 0); // D/C# = commnad 
            static uint8_t OLED_HOME[] = 
            {
              0x23, 0x00,   // disable fade-out
              0x00, 0x10,   // column address = 0x0:0x0
              0xB0          // page address = 0x0
            };
            OLED_HOME[1] = OLED_Fade_Mode;  
            if (HAL_SPI_Transmit_DMA(&hspi2,  (uint8_t *)OLED_HOME, sizeof(OLED_HOME)) != HAL_OK) Error_Handler();
            phase = 1;
            break;

          case 1: // Phase 1 of 2 -- If DMA trasnfer complete, tidy up
            if (HAL_SPI_GetState(&hspi2) != HAL_SPI_STATE_READY) break; // previous DMA transfer completed?
            HAL_GPIO_WritePin(OLED_DC_GPIO_Port, OLED_DC_Pin, 1); // D/C# = data
           
            if (HAL_SPI_Transmit_DMA(&hspi2, OLED_BUFFER, sizeof(OLED_BUFFER)) != HAL_OK) Error_Handler();
            phase = 2;
            break;

          case 2: // Phase 2 of 2 -- Wait for DMA TX to complete
            if (HAL_SPI_GetState(&hspi2) != HAL_SPI_STATE_READY) break; // previous DMA transfer completed?
            HAL_GPIO_WritePin(OLED_SS_GPIO_Port, OLED_SS_Pin, 1); // deassert OLED_SS
            phase = 0;
            STATE = VARIO_READ_TEMPERATURE; 
        }
      }
      break;
    }

    case VARIO_READ_TEMPERATURE:
    {
      static uint8_t rx_buffer[4];

      static uint8_t phase = 0;
      switch (phase)
      {
        case 0: // Phase 0 of 2 -- Request ADC conversion
        {
          static const uint8_t adc_req = 0x58;
          HAL_GPIO_WritePin(VARIO_SS_GPIO_Port, VARIO_SS_Pin, 0); // assert VARIO_SS
          if (HAL_SPI_TransmitReceive_DMA(&hspi2, (uint8_t *)&adc_req, rx_buffer, sizeof(adc_req)) != HAL_OK) Error_Handler();
          MS5611_Timer = 10; // ADC conversion takes ~8.2ms
          phase = 1;
          break;
        }

        case 1: // Phase 1 of 2 -- Initiate DMA read of ADC result
        {
          if (HAL_SPI_GetState(&hspi2) != HAL_SPI_STATE_READY) break;
          HAL_GPIO_WritePin(VARIO_SS_GPIO_Port, VARIO_SS_Pin, 1); // de-assert VARIO_SS
          if (MS5611_Timer == 0) // 10ms yet?
          {
            if (rx_buffer[0] != 254) Error_Handler(); // TODO: hard falulting for such a minor thing is a bad idea!!

            // read ADC results
            static const uint8_t read_adc = 0x00;
            HAL_GPIO_WritePin(VARIO_SS_GPIO_Port, VARIO_SS_Pin, 0); // assert VARIO_SS
            if (HAL_SPI_TransmitReceive_DMA(&hspi2, (uint8_t *)&read_adc, rx_buffer, sizeof(rx_buffer)) != HAL_OK) Error_Handler();
            phase = 2;
          }
          else
          {
            STATE = OLED_REFRESH; // yield state machine until we're ready
          }
          break;
        }

        case 2: // Phase 2 of 2 == Read and store ADC result
        {
          if (HAL_SPI_GetState(&hspi2) != HAL_SPI_STATE_READY) break;
          HAL_GPIO_WritePin(VARIO_SS_GPIO_Port, VARIO_SS_Pin, 1); // de-assert VARIO_SS
          if (rx_buffer[0] != 254) Error_Handler(); // TODO hard faulting here is a dumb idea
          MS5611_D2 = ((uint32_t)rx_buffer[1]<<16) | ((uint32_t)rx_buffer[2]<<8) | ((uint32_t)rx_buffer[3]);
          phase = 0;
          STATE = VARIO_READ_PRESSURE; // take it from the top ladies!
          break;
        }
      }
      break;
    }

    case VARIO_READ_PRESSURE:
    {
      static uint8_t rx_buffer[4];
      static uint8_t phase = 0;
      switch (phase)
      {
        case 0: // Phase 0 of 2 -- Request ADC conversion
        {
          static const uint8_t adc_req = 0x48;
          HAL_GPIO_WritePin(VARIO_SS_GPIO_Port, VARIO_SS_Pin, 0); // assert VARIO_SS
          if (HAL_SPI_TransmitReceive_DMA(&hspi2, (uint8_t *)&adc_req, rx_buffer, sizeof(adc_req)) != HAL_OK) Error_Handler();
          MS5611_Timer = 10; // ADC conversion takes ~8.2ms
          phase = 1;
          break;
        }

        case 1: // Phase 1 of 2 -- Initiate DMA read of ADC result
        {
          if (HAL_SPI_GetState(&hspi2) != HAL_SPI_STATE_READY) break;
          HAL_GPIO_WritePin(VARIO_SS_GPIO_Port, VARIO_SS_Pin, 1); // de-assert VARIO_SS
          if (MS5611_Timer == 0) // 10ms yet?
          {
            if (rx_buffer[0] != 254) Error_Handler(); // TODO: hard falulting for such a minor thing is a bad idea!!

            // read ADC results
            static const uint8_t read_adc = 0x00;
            HAL_GPIO_WritePin(VARIO_SS_GPIO_Port, VARIO_SS_Pin, 0); // assert VARIO_SS
            if (HAL_SPI_TransmitReceive_DMA(&hspi2, (uint8_t *)&read_adc, rx_buffer, sizeof(rx_buffer)) != HAL_OK) Error_Handler();
            phase = 2;
          }
          else
          {
            STATE = OLED_REFRESH; // yield state machine until we're ready
          }
          break;

        }

        case 2: // Phase 2 of 2 == Read and store ADC result
        {
          if (HAL_SPI_GetState(&hspi2) != HAL_SPI_STATE_READY) break;
          HAL_GPIO_WritePin(VARIO_SS_GPIO_Port, VARIO_SS_Pin, 1); // de-assert VARIO_SS
          if (rx_buffer[0] != 254) Error_Handler(); // TODO hard faulting here is a dumb idea
          MS5611_D1 = ((uint32_t)rx_buffer[1]<<16) | ((uint32_t)rx_buffer[2]<<8) | ((uint32_t)rx_buffer[3]);
          phase = 0;
          STATE = OLED_REFRESH; // take it from the top ladies!
          break;
        }
      }
      break;  // VARIO_READ_PRESSURE:
    }

  } // switch (STATE)


} // spiStateMachine()


