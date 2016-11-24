#include "display.h"
#include "stm32f1xx_hal.h"
#include "spi.h"
#include "main.h"
#include "stdlib.h"

uint8_t OLED_BUFFER[DISPLAY_BUFFER_SIZE];
uint8_t OLED_RESET_DONE;

volatile OLED_FadeModeTypeDef OLED_Fade_Mode = NORMAL;

#include "font_5x8.c"
#include "font_10x16.c"
Font *OLED_font = (Font *)&FONT_10x16; 
void ol_set_font(uint8_t font_index)
{
  switch (font_index)
  {
    case FONT_SMALL:
      OLED_font = &FONT_5x8;
      break;

    case FONT_LARGE:
    default:
      OLED_font = &FONT_10x16;
      break;
  }
}

volatile int OLED_X, OLED_Y = 0;

OLED_COLOR OLED_col = FG;

void setColor(OLED_COLOR col)
{
  OLED_col = col;
}

void cls() 
{
  memset(OLED_BUFFER, (uint8_t)0, sizeof(OLED_BUFFER));
  OLED_X = OLED_Y = 0;
}

void moveto(int x, int y)
{
  OLED_X = x;
  OLED_Y = y;
}

void plotc(uint16_t x, uint16_t y, OLED_COLOR col)
{
  if ((x > DISPLAY_W-1) || (y > DISPLAY_H-1)) return;
  switch (col)
  {
    case INV:
      PXOR(x, y);
      break;
    case BG: 
      PCLR(x, y); 
      break;
    case FG:
    default:
      PSET(x, y);
      break;
  }
  OLED_X = x;
  OLED_Y = y;
  // does not alter OLED_col
}

void plot(uint16_t x, uint16_t y)
{
  plotc(x, y, OLED_col);
}


// https://rosettacode.org/wiki/Bitmap/Bresenham%27s_line_algorithm#C
void line(
    uint16_t x0, 
    uint16_t y0, 
    uint16_t x1, 
    uint16_t y1)
{ 
  int dx = abs(x1-x0), sx = x0<x1 ? 1 : -1;
  int dy = abs(y1-y0), sy = y0<y1 ? 1 : -1; 
  int err = (dx>dy ? dx : -dy)/2, e2;

  for(;;){
    plot(x0,y0);
    if (x0==x1 && y0==y1) break;
    e2 = err;
    if (e2 >-dx) { err -= dy; x0 += sx; }
    if (e2 < dy) { err += dx; y0 += sy; }
  }
  OLED_X = x0;
  OLED_Y = y0;
}

void lineto(int x, int y)
{
  line(OLED_X, OLED_Y, x, y);
}

// https://rosettacode.org/wiki/Bitmap/Midpoint_circle_algorithm#C
void circle(
    uint16_t x0,
    uint16_t y0,
    uint16_t radius)
{
  int f = 1 - radius;
  int ddF_x = 0;
  int ddF_y = -2 * radius;
  int x = 0;
  int y = radius;

  plot(x0, y0 + radius);
  plot(x0, y0 - radius);
  plot(x0 + radius, y0);
  plot(x0 - radius, y0);

  while(x < y) 
  {
    if(f >= 0) 
    {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x + 1;    
    plot(x0 + x, y0 + y);
    plot(x0 - x, y0 + y);
    plot(x0 + x, y0 - y);
    plot(x0 - x, y0 - y);
    plot(x0 + y, y0 + x);
    plot(x0 - y, y0 + x);
    plot(x0 + y, y0 - x);
    plot(x0 - y, y0 - x);
  }
  OLED_X = x;
  OLED_Y = y;
}

void ol_scroll_up(int yd)
{
  // TODO: move pixels up yd lines
  OLED_Y = DISPLAY_H - yd - 1; // reset OLED_Y to top of new, blank character box 
}

int ol_putc(char ch)
{
  // Font *font_data = (Font *)&FONT_5x8;
  Font *font_data = OLED_font; 
   
  if (ch == '\r') 
  { 
    OLED_X = 0;
  }
  else if (ch == '\n')
  {
    if (OLED_Y < DISPLAY_H-1) OLED_Y += 8;
    if (OLED_Y > (DISPLAY_H/8-1)) ol_scroll_up(8);
  }
  else 
  {
    // copy one character's font data to screen buffer for display
    uint8_t y_shift = OLED_Y % 8;               // character may span two or more 8bit display rows
    uint8_t bits;                               // one 8 bit high display column
    for (uint8_t row = 0; row < (font_data->rows / 8); row++ ) // font rows (if more than one)
    {
      uint8_t ci = ch - 0x20;                                         // character index (left to right, top to bottom grid, in font bitmap) 
      const uint8_t cpl = font_data->width / font_data->cols;         // characters per line in bitmap
      uint8_t cr = ci / cpl;                                          // character's row
      uint8_t cp = ci % cpl;                                          // character's position in row
      const uint16_t bpl = (font_data->rows / 8) * font_data->width;  // bytes per 8-bit row (total)
      uint16_t char_idx = (cr * bpl) + (cp * font_data->cols);        // index of upper-left cell of character in font bitmap
      uint8_t *fnt = &font_data->data[char_idx] + (row * font_data->width);
      uint8_t *dsp = &OLED_BUFFER[ OLED_X + (DISPLAY_W * (OLED_Y / 8)) + (row * DISPLAY_W) ];
      for (int c = 0; c < font_data->cols; c++ ) // c = font column
      {
        uint8_t *src = fnt + c;
        uint8_t *dst = dsp + c;
        if (dst > DISPLAY_END) continue;

        if (1) { } // TODO detect buffer OR screen edge overruns, wrap to next line etc
        
        // the following takes care of display row staddles, having nothing to do with the font data
        bits = *dst & ~(0xFF << y_shift); // get display byte, mask off new character's upper bits, preversing others
        bits |= *src << y_shift;          // copy in new character's upper bits
        *dst = bits;                      // write the result back to display buffer
        if (y_shift)
        {
          dst = dst + DISPLAY_W;
          if (dst > DISPLAY_END) continue;
          bits = *dst & ~(0xFF >> (8-y_shift));
          bits |= *src >> (8 - y_shift);
          *dst = bits;
        }
      }
    }
    
    OLED_X += font_data->cols + 1 * (font_data->rows / 8); // letter spacing nonsense

    if (OLED_X > DISPLAY_W-1)                   // gone past end of display row? 
    {
      OLED_X = 0;                               // home "cursor" and advance one row
      if (OLED_Y > (DISPLAY_H/8-1)) 
      {
        ol_scroll_up(font_data->rows); // TODO: function (above) implemented but does nothings
      }
    }
  }
  return ch;
}

char *ol_puts(char *s)
{
  while(*s) ol_putc(*s++);
  return s;
}

char *ol_puts_centered(char *s)
{
  // TODO: support different size fonts -- some kind of 'current font' or perhaps supply a Font type arg?
  // For now, just go with the 10x16
  const int width = 10;   // of character
  const int height = 16;  //     "
  int x = DISPLAY_W / 2 - strlen(s) * width / 2;
  int y = DISPLAY_H / 2 - height / 2;
  moveto(x, y);
  return ol_puts(s);
}


void OLED_init()
{
  cls(); // clear screen (our own buffer, not the OLED itself)

  // Initialise the OLED screen
  // power up Vbat, after a short delay
  HAL_Delay(5);
  HAL_GPIO_WritePin(OVBAT1_GPIO_Port, OVBAT1_Pin, 1);
  HAL_GPIO_WritePin(OVBAT2_GPIO_Port, OVBAT2_Pin, 1);
  HAL_GPIO_WritePin(OLED_RES_GPIO_Port, OLED_RES_Pin, 0);
  HAL_Delay(1);
  HAL_GPIO_WritePin(OLED_RES_GPIO_Port, OLED_RES_Pin, 1);
  HAL_Delay(100);
  static const uint8_t OLED_INIT[] = {
    0xA1,         // remap mode
    0xA8,         // multiplex ratio
    0x1F,         // duty = 1/32
    0xC8,         // COM scan direction
    0xDA, 0x00,   // COM pins
    0xD3, 0x00,   // diplay offset
    0xD5, 0x80,   // OSC division
    0xDB, 0x40,   // VCOMH
    0xD9, 0x1F,   // pre-charge period
    0x81, 0x7F,   // set contrast to mid-point
    0x20, 0x00,   // horrizontal addressing mode
    0xA6,         // normal display (white on black)
    0x23, 0x00,   // disable fade-out
    0x8D, 0x14,   // charge pump enable MUST BE LAST and be followed by 100ms delay
    0xAF          // display on
  };
  HAL_GPIO_WritePin(OLED_SS_GPIO_Port, OLED_SS_Pin, 0); // assert OLED_SS 
  HAL_GPIO_WritePin(OLED_DC_GPIO_Port, OLED_DC_Pin, 0); // D/C# = commnad
  if (HAL_SPI_Transmit(&hspi2, (uint8_t *)OLED_INIT, sizeof(OLED_INIT), HAL_MAX_DELAY) != HAL_OK) Error_Handler();
  HAL_GPIO_WritePin(OLED_SS_GPIO_Port, OLED_SS_Pin, 1); // de-assert OLED_SS 
  HAL_Delay(100); // allow 100ms for charge pump Vcc power to stabalise
  
  OLED_RESET_DONE = 1;
}

