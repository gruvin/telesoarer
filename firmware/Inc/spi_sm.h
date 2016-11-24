#ifndef __SPI_SM_H
#define __SPI_SM_H

#include "main.h"
#include "stm32f1xx_hal.h"
#include "dma.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include <string.h> // memset
#include "ms5611.h" // [global] unint16_t MS5611_Timer;

extern void spiStateMachine();
  
#endif // __SPI_SM_H
