#include "oled.hpp"

U8G2_SSD1306_128X64_NONAME_F_HW_I2C oled(U8G2_R0, 21, 22);

uint8_t bootStep;

void setFont() {
  oled.setDrawColor(1);
  // oled.setFont(u8g2_font_ncenB08_tr);
  oled.setFont(u8g2_font_5x8_tf);
  oled.setFontPosTop();
}

void oledSetup() {
  // if (!gui) { return; }
  bootStep = 0;
  log("[OLED] Setup... ");
  delay(50);
  oled.begin();
  setFont();
  gui = true;
  done();
}

void drawBootStep(const char *name) {
  uint8_t w = oled.drawStr(4, 4 + bootStep * 10, name);
  oled.drawStr(4 + w, 4 + bootStep * 10, "...");
  draw();
}

void drawBootDone() { oled.drawStr(64, 4 + bootStep++ * 10, "OK"); draw(); }
void drawBootFail() { oled.drawStr(64, 4 + bootStep++ * 10, "FAIL"); draw(); }
void stepBoot() { bootStep++; draw(); }

void drawTest() {
  oled.clearBuffer();					// clear the internal memory
  oled.setFont(u8g2_font_ncenB08_tr);	// choose a suitable font
  oled.drawStr(0,10,"Hello World!");	// write something to the internal memory
  oled.sendBuffer();					// transfer internal memory to the display
  delay(1000);
}

void drawProgressBar(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t percent) {
  if (!gui) { return; }
  height = height < 8 ? 8 : height;
  width = width < 10 ? 10 : width;
  oled.setFont(u8g2_font_5x8_tf);
  oled.setFontPosBaseline();
  oled.drawStr(x + width / 2 - 5 * 1, y - 1, (String(percent) + String("%")).c_str());
  oled.drawRFrame(x, y, width, height, 4);
  oled.drawBox(x + 2, y + 2, (width - 4) * (percent / 100.0), height - 4 );
  setFont();
}

void drawString(const char *str, uint8_t x, uint8_t y) {
  if (!gui) { return; }
  oled.drawStr(x, y, str);
}

void draw()  { if (!gui) { return; }; oled.sendBuffer(); }
void clear() { if (!gui) { return; }; oled.clearBuffer(); }
void oledOff() { if (!gui) { return; } oled.setPowerSave(1); }
void oledOn() { if (gui) { oledSetup(); } }
