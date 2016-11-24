#include "morse.h"
#include "main.h"
#include "stm32f1xx_hal.h"
#include "tim.h"
#include <stdio.h>

// LONG LIVE THE CODE! LONG LIVE THE CODE! LONG LIVE THE CODE!
// ..-. --- -. --.   ..-. .. ...- .  - .... .   -.-. --- -.. .  -.-.--
// di'di'dahdit da'da'dah dahdit da'dahdit
// di'di'dahdit di'dit di'di'di'dah dit
// dah di'di'di'dit dit
// da'di'dahdit da'da'dah dahdi'dit dit
// da'di'da'di'da'dah
const unsigned char morseTable[47] = {
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


volatile enum morseState_t { 
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
char *morseString;

void morseSend(char *s)
{
  if (morseState == STOP)
  {
    morseString = s;
    morseState = START;
  }
}
int isMorseReady()
{
  return (morseState == STOP);
}

void morseStop()
{
  beeperOff();
  morseState = STOP;
}

void morseStateMachine() 
{
  static unsigned int timer;
  static unsigned char bitCount;
  static unsigned char letterBits;
  static unsigned char charIndex;
  static char thisChar;

  switch (morseState)
  {
    case DIT:
      if (timer == 0)
        beeperOn();
      if (++timer > DITLENGTH) 
      {
        timer = 0;
        morseState = BITGAP;
      }
      break;

    case DAH:
      if (timer == 0)
        beeperOn();
      if (++timer > DITLENGTH * 3)
      {
        timer = 0;
        morseState = BITGAP;
      }
      break;

    case BITGAP:
      if (timer == 0) 
        beeperOff();
      if (++timer > DITLENGTH) morseState = NEXTBIT;
      break;

    case LETTERGAP:
      if (timer == 0) 
        beeperOff();
      if (++timer > DITLENGTH * 2) morseState = NEXTCHAR;
      break;

    case SPACE:
      if (timer == 0) 
        beeperOff();
      if (++timer > DITLENGTH * 6) morseState = NEXTCHAR;
      break;

    case NEXTBIT:

      timer = 0;

      if (letterBits & 0b10000000)
        morseState = DAH;
      else
        morseState = DIT;

      letterBits <<= 1; // shift bits left 1 position
      if (bitCount-- == 0)
        morseState = LETTERGAP;

      break;

    case NEXTCHAR:

      thisChar = morseString[charIndex++];

      if (thisChar == ' ') 
      {
        timer = 0;
        morseState = SPACE;
      }
      else if ( (thisChar == '\0') || ((thisChar < ',') && (thisChar != '!')) || (thisChar > 'Z') ) // end of string or invalid character?  
      {
        morseState = STOP;
      }
      else
      {
        letterBits = (
          (thisChar == '!') ?
            0b11101011 // '!'
            : 
            morseTable[thisChar-44]
        );
        bitCount = (letterBits >> 5);
        if (bitCount < 6)
          letterBits <<= 3;
        else
          letterBits <<= 2;

        morseState = NEXTBIT;
      }
      break;


    case START:
      beeperOff();
      timer = 0;
      charIndex = 0;
      morseState = NEXTCHAR;
      break;

    case STOP:
      break; // do nothing

    default:
      morseState = STOP;
      break;

  } // switch
      
}

