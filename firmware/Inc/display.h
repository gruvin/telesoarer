#ifndef __DISPLAY_H
#define __DISPLAY_H

#include <stdint.h>
#include <string.h>
#include "fonts.h"

#define DISPLAY_W 128
#define DISPLAY_H 32
#define DISPLAY_BUFFER_SIZE (DISPLAY_W * DISPLAY_H / 8) // 4 pages (rows) each containing 128 bytes (8-bit high coloumns)
#define DISPLAY_END (OLED_BUFFER + DISPLAY_BUFFER_SIZE)
#define PSET(x,y) OLED_BUFFER[x + ((y & ~7) << 4)] |=  (1<<(y%8))
#define PCLR(x,y) OLED_BUFFER[x + ((y & ~7) << 4)] &= ~(1<<(y%8))
#define PXOR(x,y) OLED_BUFFER[x + ((y & ~7) << 4)] ^= (1<<(y%8))
#define FG 1      // foreground color code 
#define BG 0      // background color code
#define INV (-1)  // inverse color code
#define FONT_SMALL 0
#define FONT_LARGE 1

extern uint8_t OLED_BUFFER[DISPLAY_BUFFER_SIZE];
extern uint8_t OLED_RESET_DONE;

extern Font *OLED_font;
extern volatile int OLED_X, OLED_Y;
typedef int8_t OLED_COLOR; // signed to occodate -1 (inverse)
typedef enum
{
  NORMAL = 0x00,
  FADE_FAST = 0x20,
  FADE_SLOW = 0x24,
  BLINK = 0x31
} OLED_FadeModeTypeDef;
extern volatile OLED_FadeModeTypeDef OLED_Fade_Mode;

extern void ol_set_font(uint8_t font_index);
extern void setColor(OLED_COLOR col);
extern void cls();
extern void moveto(int x, int y);
extern void plotc(uint16_t x, uint16_t y, OLED_COLOR col);
extern void plot(uint16_t x, uint16_t y);
extern void line(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
extern void lineto(int x, int y);
extern void circle(uint16_t x0, uint16_t y0, uint16_t radius);
extern void ol_scroll_up(int yd);
extern int ol_putc(char ch);
extern char *ol_puts(char *s);
extern char *ol_puts_centered(char *s);
extern void OLED_init();

#endif // __DISPLAY_H
