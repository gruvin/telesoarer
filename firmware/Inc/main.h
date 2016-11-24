/**
  ******************************************************************************
  * File Name          : main.h
  * Description        : This file contains the common defines of the application
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H
  /* Includes ------------------------------------------------------------------*/

/* USER CODE BEGIN Includes */
#include <stdint.h>
/* USER CODE END Includes */

/* Private define ------------------------------------------------------------*/
#define BAUD_RATE 115200
#define TIM2_PRESCALE 72000000/256/2800
#define TIM3_PRESCALE 72000000/1024/2000
#define TIM4_PRESCALE ((72/2)-1)

#define PIEZO_Pin GPIO_PIN_3
#define PIEZO_GPIO_Port GPIOA
#define OLED_SS_Pin GPIO_PIN_4
#define OLED_SS_GPIO_Port GPIOA
#define OLED_RES_Pin GPIO_PIN_5
#define OLED_RES_GPIO_Port GPIOA
#define OLED_DC_Pin GPIO_PIN_6
#define OLED_DC_GPIO_Port GPIOA
#define VARIO_OUT_Pin GPIO_PIN_7
#define VARIO_OUT_GPIO_Port GPIOA
#define LED_RED_Pin GPIO_PIN_0
#define LED_RED_GPIO_Port GPIOB
#define LED_BLUE_Pin GPIO_PIN_1
#define LED_BLUE_GPIO_Port GPIOB
#define LED_GREEN_Pin GPIO_PIN_2
#define LED_GREEN_GPIO_Port GPIOB
#define VARIO_SS_Pin GPIO_PIN_12
#define VARIO_SS_GPIO_Port GPIOB
#define OVBAT1_Pin GPIO_PIN_11
#define OVBAT1_GPIO_Port GPIOA
#define OVBAT2_Pin GPIO_PIN_12
#define OVBAT2_GPIO_Port GPIOA
#define SIG_Pin GPIO_PIN_7
#define SIG_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

#define USE_FULL_ASSERT    1
#define INACTIVIY_PERIOD 10000 // ms -- how long to wait before sounding "lost" alarm
#define SIG_MAX_PERIOD    25   // ms -- consider sigPulseWidth to be invalid if we've not has any pulse for this long

// assuming we have user named GPIO pins defined above as LED_RED, LED_GREEN, LED_BLUE etc ... 
#define LED_ON(a)     HAL_GPIO_WritePin(LED_ ## a ## _GPIO_Port, LED_ ## a ## _Pin, 1)
#define LED_OFF(a)    HAL_GPIO_WritePin(LED_ ## a ## _GPIO_Port, LED_ ## a ## _Pin, 0)
#define LED_TOGGLE(a) HAL_GPIO_TogglePin(LED_ ## a ## _GPIO_Port, LED_ ## a ## _Pin)


// globals acted on by ISRs
extern volatile int16_t sigPulseWidth;  // TIM4 IC ISR
extern volatile int16_t sigPulseWidth;  // TIM4 IC ISR
extern volatile int16_t sigPulsePeriod; // TIM4 IC ISR
extern volatile uint16_t auxTimer;      // millisecond down counter (stops at zero)
extern volatile uint16_t activityTimer; // millisecond down counter (stops at zero)

extern void beeperOn();
extern void beeperOff();

/* USER CODE END Private defines */

/**
  * @}
  */ 

/**
  * @}
*/ 

#endif /* __MAIN_H */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
