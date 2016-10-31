#ifndef MS5611_H
#define MS5611_H

#include <SPI.h>

#define  MS5611_SS (10)

extern void MS5611_init();
extern void MS5611_reset();
extern void MS5611_loadPROM();
extern uint32_t getRawPressure();
extern uint32_t getRawTemperature();
extern uint32_t getPressure();
extern double getLastTemp();


#endif

