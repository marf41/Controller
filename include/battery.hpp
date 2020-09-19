#include <Arduino.h>
#include <esp_adc_cal.h>

#define BATTERY_MIN_V 3.3
#define BATTERY_MAX_V 4.2

#define BATT_ADC_PIN 35

void batterySetup();
float getVoltage();
uint8_t calcPercentage(float volts);