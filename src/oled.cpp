#include "oled.hpp"

U8G2_SSD1306_128X64_NONAME_F_HW_I2C oled(U8G2_R0, 21, 22);

void oledSetup() {
    delay(50);
    oled.begin();
    oled.setDrawColor(1);
}

void drawProgressBar(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t percent) {
  height = height < 8 ? 8 : height;
  width = width < 10 ? 10 : width;
  oled.setFont(u8g2_font_5x8_tf);
  oled.drawStr(x + width / 2 - 5 * 1, y - 1, (String(percent) + String("%")).c_str());
  oled.drawRFrame(x, y, width, height, 4);
  oled.drawBox(x + 2, y + 2, (width - 4) * (percent / 100.0), height - 4 );
}

void drawString(const char *str, uint8_t x, uint8_t y) {
    oled.drawStr(x, y, str);
}

void draw() { oled.sendBuffer(); }
void clear() { oled.clearBuffer(); }
