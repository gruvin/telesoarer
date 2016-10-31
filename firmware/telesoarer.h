#ifndef ALTITUDE88_H
#define ALTITUDE88_H

#define SIG_PIN 8 /* ICR1 */
#define PIEZO_PIN 5
#define PIEZO_FREQ 2800

#define ALT_PIN 3 // altitude PWM output pin -- PD2/OC2B

#define LED1  A2 // Arduino I/O pin numbers
#define LED2  A3
#define LED3  A4
#define LED_ON(a) digitalWrite(a, HIGH);
#define LED_OFF(a) digitalWrite(a, LOW);
#define LED_TOGGLE(a) digitalWrite(a, !digitalRead(a))

#define MIDPOINT 1500 // uS
#define PULSE_TIMEOUT 23 // mS

// PWM piezo output volume
#ifdef DEBUG
#define PWM_DUTY_CYCLE 3
#define PWM_DUTY_CYCLE_QUIET 2
#else
#define PWM_DUTY_CYCLE 27
#define PWM_DUTY_CYCLE_QUIET 10
#endif

// CW dit length (arbitrary units)
#define DITLENGTH 90 // ms

//#define true (-1)
//#define false 0

const unsigned char morseTable[47] PROGMEM = {
  0b11110011, // , ASCII 44(dec)
  0,
  0b11010101, // .
  0b10110010, // /
  0b10111111, // 0
  0b10101111, // 1
  0b10100111, // 2
  0b10100011, // 3
  0b10100001, // 4
  0b10100000, // 5
  0b10110000, // 6
  0b10111000, // 7
  0b10111100, // 8
  0b10111110, // 9
  0,          // :
  0,          // ;
  0,          // <
  0b10110001, // =
  0,          // >
  0b11001100, // ?
  0,          // @
  0b01001000, // A
  0b10010000, // B
  0b10010100, // C
  0b01110000, // D
  0b00100000, // E
  0b10000100, // F
  0b01111000, // G
  0b10000000, // H
  0b01000000, // I
  0b10001110, // J
  0b01110100, // K
  0b10001000, // L
  0b01011000, // M
  0b01010000, // N
  0b01111100, // O
  0b10001100, // P
  0b10011010, // Q
  0b01101000, // R
  0b01100000, // S
  0b00110000, // T
  0b01100100, // U
  0b10000010, // V
  0b01101100, // W
  0b10010010, // X
  0b10010110, // Y
  0b10011000  // Z
}; // morse table


enum morseState_t { 
  END = 0,
  DIT,
  DAH,
  BITGAP,
  LETTERGAP,
  SPACE,
  NEXTBIT,
  NEXTCHAR,
  START,
  STOP
} morseState = STOP;

#endif // ALTITUDE88_H

