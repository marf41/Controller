#include <U8x8lib.h>
#include <u8x8.h>
#include <Wire.h>

void oledSetup();
void drawProgressBar(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t percent);
void drawString(const char *str, uint8_t x, uint8_t y);
void draw();
void clear();
