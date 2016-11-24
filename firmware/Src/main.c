/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
  ******************************************************************************
  *
  * COPYRIGHT(c) 2016 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f1xx_hal.h"
#include "dma.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <math.h> // sin (just for testing OLED)
#include "lost_sm.h"
#include "spi_sm.h"
#include "display.h"
#include "ms5611.h"
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
volatile int16_t sigPulseWidth = (-1);
volatile int16_t sigPulsePeriod = (-1);
volatile uint16_t activityTimer = 0;
volatile uint16_t auxTimer = 0;
// #define DEBUG 1
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void Error_Handler(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/
#ifdef __GNUC__
/* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
   set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
/* USER CODE END PFP */

/* USER CODE BEGIN 0 */
void beeperOn()
{
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_4);
  LED_ON(GREEN);
}

void beeperOff()
{
  HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_4);
  LED_OFF(GREEN);
}

/* USER CODE END 0 */

int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART1_UART_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_SPI2_Init();

  /* USER CODE BEGIN 2 */

  OLED_init();
  VARIO_init();

  static Lost_State lastLostState;
  lastLostState = LOST_STATE;

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    if (sigPulseWidth > 999) printf("%d\r\n", sigPulseWidth);

    ol_set_font(FONT_LARGE);
    if (LOST_STATE == LS_BOOTING || lastLostState != LOST_STATE)
    {
      cls();
      ol_set_font(1);
      switch (LOST_STATE)
      {
        case LS_BOOTING:
          ol_puts_centered("HELLO");
          break;

        case LS_NOSIG:
          ol_puts_centered("NO SIGNAL");
          break;

        case LS_ASK_PROG:
          ol_puts_centered("PROGRAM?");
          break;

        case LS_PROG_WAIT:
          ol_puts_centered("STICK UP!");
          break;

        case LS_PROGRAMMING:
          ol_puts_centered("PROGRAM!");
          break;

        case LS_PROG_NORMAL:
          ol_puts_centered("MODE NOR?");
          break;

        case LS_PROG_INACTIVITY:
          ol_puts_centered("MODE INA?");
          break;

        case LS_PROG_OK:
          ol_puts_centered("OK");
          break;

        case LS_ARMED_NORMAL:
        case LS_ARMED_INACTIVITY:
        {
          //TODO: enable auto fade on OLED

          static char s_armed[] = "ARMED";
          // center horrizontally
          moveto( (DISPLAY_W / 2) - (sizeof(s_armed) * OLED_font->cols / 2), 0 ); 
          ol_puts(s_armed);
          break;
        }

        case LS_LOST:
        {
          static char s_lost[] = "I'm LOST!";
          moveto( (DISPLAY_W / 2) - (sizeof(s_lost) * OLED_font->cols / 2), 0 ); 
          ol_puts(s_lost);
          break;
        }

      } // switch (LOST_STATE)
      lastLostState = LOST_STATE;
    } // if
    
    switch (LOST_STATE)
    {
        case LS_ARMED_NORMAL:
        case LS_ARMED_INACTIVITY:
        case LS_LOST:
        {
          static char s_normal[] = "NORM";
          static char s_inactivity[] = "INACT";
          ol_set_font(FONT_SMALL); // change to small font
          moveto(0, 24); ol_puts( (runMode == RM_NORMAL) ? s_normal : s_inactivity );

          // calulate sensor air pressure, in milli-Bar
          int64_t dT = (int64_t)MS5611_D2 - ((int64_t)MS5611_C[5] << 8);
          int64_t offset = ((int64_t)MS5611_C[2] << 16) + (((int64_t)MS5611_C[4] * dT) >> 7);
          int64_t sens = ((int64_t)MS5611_C[1] << 15) + (((int64_t)MS5611_C[3] * dT) >> 8);
          int64_t T = 2000 + ((int64_t)dT * ((int64_t)MS5611_C[6]) >> 23);
          int64_t T2 = 0, offset2 = 0, sens2 = 0;  
          if (T < 2000) {
            T2 = (dT * dT) >> 31;
            offset2 = 5 * (int64_t)((T - 2000) * (T - 2000)) >> 1;
            sens2 = 5 * (int64_t)((T - 2000) * (T - 2000)) >> 2;
            if (T < -1500) {
              offset2 += 7 * ((T + 1500) * (T + 1500));
              sens2 += 11 * ((T + 1500) * (T + 1500)) >> 1;
            }
          }
          offset = offset - offset2;
          sens = sens - sens2;

          P = ((MS5611_D1 * sens >> 21) - offset) >> 15;  // pressure in mBar (x100)
          TEMP = (T - T2);                                // temperature in degrees Celcius (x100)

          char buf[10];

          sprintf(buf, "%-2d.%1d@C", TEMP / 100, TEMP / 10 % 10);
          moveto( 6*7, 24 );
          ol_puts(buf);

          sprintf(buf, "%4.4u.%02u", (unsigned)P / 100, (unsigned)P % 100);
          moveto(127-8*OLED_font->cols, 24);
          ol_puts(buf);
          break;
        }

        default:
          break;
      }


    HAL_Delay(10);

  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */
  } // while(1) main loop
  /* USER CODE END 3 */

}

/** System Clock Configuration
*/
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* USER CODE BEGIN 4 */

/**
 * @brief  Retargets the C library printf function to the USART. Called by syscalls.c:_write
 * @param  None
 * @retval None
 */
PUTCHAR_PROTOTYPE
{
  //ol_putc((char)ch);
  HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, -1);
  return ch;
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler */
  /* User can add his own implementation to report the HAL error return state */
  /* We should diable ALL interrupts here. BUt I cannot find an instruction for that?! Maybe
     I'm just supposed to manually write int/event mask resgister(s) to all zeroes? */
  while(1) 
  {
    LED_TOGGLE(RED);
    LED_TOGGLE(BLUE);
    LED_TOGGLE(GREEN);
    HAL_Delay(50);
  }
  /* USER CODE END Error_Handler */ 
}

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  printf("ERROR in file %s at line %d", file, (int)line);
  Error_Handler(); // from whence we shall not return
  /* USER CODE END 6 */

}

#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
