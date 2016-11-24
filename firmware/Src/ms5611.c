#include "ms5611.h"
#include <string.h> // memcpy

uint16_t MS5611_C[8]; // [sensor_number][Cn]
uint32_t MS5611_D1;
uint32_t MS5611_D2;
volatile int16_t TEMP = 0; // temperature in ºC (x100)
volatile uint32_t P = 0; // air pressure in mBat (x100)volatile double MS5611_TEMP;
volatile uint16_t MS5611_Timer;
uint8_t VARIO_RESET_DONE = 0;


void VARIO_init()
{
  uint8_t rx_buffer[sizeof(MS5611_C)];

  // reset command
  HAL_GPIO_WritePin(VARIO_SS_GPIO_Port, VARIO_SS_Pin, 0); // assert VARIO_SS
  
  uint8_t command = 0x1E; // reset
  if (HAL_SPI_TransmitReceive(&hspi2, (uint8_t *)&command, rx_buffer, 1,  MS5611_TIMEOUT) != HAL_OK) Error_Handler();
  HAL_GPIO_WritePin(VARIO_SS_GPIO_Port, VARIO_SS_Pin, 1); // de-assert VARIO_SS
  if (rx_buffer[0] != 254) Error_Handler(); // ouch! She'll be right!
  HAL_Delay(10);

  uint8_t addr = 0xA0; // load PROM
  HAL_GPIO_WritePin(VARIO_SS_GPIO_Port, VARIO_SS_Pin, 0); // assert VARIO_SS
  if (HAL_SPI_TransmitReceive(&hspi2, (uint8_t *)&addr, rx_buffer, sizeof(addr), MS5611_TIMEOUT) != HAL_OK) Error_Handler();
  HAL_GPIO_WritePin(VARIO_SS_GPIO_Port, VARIO_SS_Pin, 1); // de-assert VARIO_SS
  if (rx_buffer[0] != 254) Error_Handler(); // ouch! Awe, geeze mate! Fair crack o' da' whip!
  for (int i=0; i < 8; i++)
  {
    HAL_GPIO_WritePin(VARIO_SS_GPIO_Port, VARIO_SS_Pin, 0); // assert VARIO_SS
    if (HAL_SPI_TransmitReceive(&hspi2, (uint8_t *)&addr, (uint8_t *)&MS5611_C[i], 2, MS5611_TIMEOUT) != HAL_OK) Error_Handler();
    HAL_GPIO_WritePin(VARIO_SS_GPIO_Port, VARIO_SS_Pin, 1); // de-assert VARIO_SS
    addr += 2;
  }
  // TODO: Pretty sure there's a checksum we can look at in there somewhere?

  VARIO_RESET_DONE = 1;
}



