#include "battery.hpp"

int vref = 1100;

struct voltagePoint {
    float max;
    uint8_t per;
};

voltagePoint batteryCurve[] = {
    { 4.20, 100 },
    { 4.12, 97 },
    { 4.10, 95 },
    { 4.05, 90 },
    { 3.95, 80 },
    { 3.82, 60 },
    { 3.75, 40 },
    { 3.68, 20 },
    { 3.60, 10 },
    { 3.48, 5 },
    { 3.30, 2 },
    { 3.00, 0 },
    { 0.00, 0 },
};


void batterySetup() {
  esp_adc_cal_characteristics_t adc_chars;
  esp_adc_cal_value_t val_type = esp_adc_cal_characterize((adc_unit_t)ADC_UNIT_1, (adc_atten_t)ADC1_CHANNEL_6, (adc_bits_width_t)ADC_WIDTH_BIT_12, 1100, &adc_chars);
  if (val_type == ESP_ADC_CAL_VAL_EFUSE_VREF) { vref = adc_chars.vref; }
}

float getVoltage() {
  uint16_t v = analogRead(BATT_ADC_PIN);
  float battery_voltage = ((float)v / 4095.0) * 2.0 * 3.3 * (vref / 1000.0);
  return battery_voltage;
}

uint8_t calcPercentage(float voltage) {
  // float percentage = (volts - BATTERY_MIN_V) * 100 / (BATTERY_MAX_V - BATTERY_MIN_V);
  if (voltage >= 4.2) { return 100; }
  if (voltage <= 3.0) { return 0; }
  for (uint8_t i = 0; batteryCurve[i].max > 0; i++) {
      if (voltage >= batteryCurve[i].max) {
          float max = batteryCurve[i].max; uint8_t per = batteryCurve[i].per;
          float vr = batteryCurve[i > 0 ? (i-1) : 0].max - max;
          float vp = (voltage - max) / vr;
          uint8_t pr = batteryCurve[i > 0 ? (i-1) : 0].per - per;
          float pp = per + pr * vp;
          if (pp > 100) { return 100; }
          if (pp < 0) { return 0; }
          return (uint8_t)pp;
      }
  }
  return 0;
}