#include <U8g2lib.h>
#include <Wire.h>

#include "config.hpp"

extern bool gui;
extern bool debug;

void drawBootStep(const char *name);
void drawBootDone();
void drawBootFail();
void stepBoot();

void oledSetup();
void drawProgressBar(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t percent);
void drawString(const char *str, uint8_t x, uint8_t y);

void oledOn();
void oledOff();
void draw();
void clear();

void drawTest();