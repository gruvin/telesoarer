#ifndef __FONTS_H
#define __FONTS_H

typedef struct Fonts {
  uint8_t cols;     // width of one character, in pixels
  uint8_t rows;     // height of one character, in pixels
  uint16_t width;   // bitmap width in pixels
  uint16_t height;  // bitnapo height in pixels
  uint8_t data[];   // character data, in one long row of character 8bit columns, each 1 pixel wide
                    // fonts taller than 8 pixels contain 2 or more rows, each row appearing after 
                    // the first, in classic left-to-right, top-to-bottom style
} Font;

#endif // __FONTS_H
