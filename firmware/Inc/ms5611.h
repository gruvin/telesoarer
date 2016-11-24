#ifndef __MS5611_H
#define __MS5611_H

#include <SPI.h>

#define MS5611_TIMEOUT 10 /* mS */

extern uint8_t VARIO_RESET_DONE;
extern uint16_t MS5611_C[8];
extern uint32_t MS5611_D1; // raw pressure reading
extern uint32_t MS5611_D2; // raw temperature reading
extern volatile int16_t TEMP; // temperature in ºC (x100)
extern volatile uint32_t P; // air pressure in mBat (x100)
extern volatile uint16_t MS5611_Timer;

extern void VARIO_init();


#endif // __MS5611_H 

