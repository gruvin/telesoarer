#include "ms5611.h"

word MS5611_C[8]; // [sensor_number][Cn]
double g_lastTemp;

#ifdef DEBUG
#define DIE(a) die(a)
#else
#define DIE(a) die();
#endif
#ifdef DEBUG
void die(const __FlashStringHelper* msg) {
#else
void die() {
#endif
#ifdef DEBUG
  Serial.print(F("HALT! - "));
  Serial.println(msg);
#endif
  static bool t = false;
  while (1) { 
    t = !t;
    digitalWrite(A1, t); 
    _delay_ms(200);
  }
}

void MS5611_init() {
    // initalize the MS5611's data ready and chip select pins:
  pinMode(MS5611_SS, OUTPUT);
  digitalWrite(MS5611_SS, HIGH);
  
  // start the SPI library:
  SPI.begin();
  SPI.setDataMode(SPI_MODE3);
  SPI.setClockDivider(SPI_CLOCK_DIV2);
  SPI.setBitOrder(MSBFIRST);
  MS5611_reset();
  MS5611_loadPROM();
}

void MS5611_reset() {
  // reset the MS5611
  digitalWrite(MS5611_SS, LOW);
  if (SPI.transfer(0x1E) != 254) DIE(F("MS5611 reset failed."));
  _delay_ms(10);
  digitalWrite(MS5611_SS, HIGH);
  
}

void MS5611_loadPROM() {
  // read PROM
  byte addr=0xA0;
  for (int i=0; i<8; i++) {
    digitalWrite(MS5611_SS, LOW);
    if (SPI.transfer(addr) != 254) { DIE(F("MS5611 PROM failed.")); }
    MS5611_C[i] = SPI.transfer16(0);
    digitalWrite(MS5611_SS, HIGH);  
    addr += 2;
  }
}

uint32_t getRawPressure() {
  
  // request MS5611 conversion (D1:4096)
  digitalWrite(MS5611_SS, LOW);
  if (SPI.transfer(0x48) != 254) DIE(F("MS5611 D1 convert request failed."));
  digitalWrite(MS5611_SS, HIGH);
  _delay_ms(10); // conversion takes around 8.2ms
  
  // ADC read
  digitalWrite(MS5611_SS, LOW);
  if (SPI.transfer(0x00) != 254) DIE(F("MS5611 ADC read failed."));
  byte H8 = SPI.transfer(0);
  byte M8 = SPI.transfer(0);
  byte L8 = SPI.transfer(0);
  digitalWrite(MS5611_SS, HIGH);

  uint32_t D1 = ((uint32_t)H8<<16) | ((uint32_t)M8<<8) | ((uint32_t)L8);

  return D1;
}

uint32_t getRawTemperature() {
  // request MS5611 conversion (D2:4096)
  digitalWrite(MS5611_SS, LOW);
  if (SPI.transfer(0x58) != 254) DIE(F("MS5611 D2 convert request failed."));
  digitalWrite(MS5611_SS, HIGH);
  _delay_ms(10); // conversion takes around 8.2ms
  
  // ADC read
  digitalWrite(MS5611_SS, LOW);
  if (SPI.transfer(0x00) != 254) DIE(F("MS5611 ADC read failed."));
  byte H8 = SPI.transfer(0);
  byte M8 = SPI.transfer(0);
  byte L8 = SPI.transfer(0);
  digitalWrite(MS5611_SS, HIGH);

  uint32_t D2 =  ((uint32_t)H8<<16) | ((uint32_t)M8<<8) | ((uint32_t)L8);

  return D2;
}

uint32_t getPressure() {

  uint32_t D1 = getRawPressure();
  uint32_t D2 = getRawTemperature();
  
  int64_t dT = (int64_t)D2 - ((int64_t)MS5611_C[5] << 8);
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

  g_lastTemp = ((double)(T - T2) / 100);

  uint32_t P = ((D1 * sens >> 21) - offset) >> 15;

  return P;
}

double getLastTemp() {
  return g_lastTemp;
}

