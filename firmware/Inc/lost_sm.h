#ifndef __LOST_SM_H
#define __LOST_SM_H

#include "main.h"
#include "stm32f1xx_hal.h"
#include "dma.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include <stdlib.h> // abs()

// LOST_STATES is used to communicate gross state, globally
typedef enum
{
  LS_BOOTING=0,
  LS_NOSIG,
  LS_ASK_PROG,
  LS_PROG_WAIT,
  LS_PROGRAMMING,
  LS_PROG_NORMAL,
  LS_PROG_INACTIVITY,
  LS_PROG_OK,
  LS_ARMED_NORMAL,
  LS_ARMED_INACTIVITY,
  LS_LOST
} Lost_State;

typedef enum
{
  RM_NORMAL,
  RM_INACTIVITY
} runModeTypeDef;
extern Lost_State LOST_STATE; 
extern volatile runModeTypeDef runMode;
extern void lostModelStateMachine();
  
#endif // __SPI_SM_H
