
#include "led.hpp"

Led led(5, 1);

void ledOnBoot() {
    led.setPeriod(1000);
    led.pulse().start();
}

void ledOnStandby() {
    led.off();
    led.setDuration(10);
    led.setInterval(5000);
    led.blink().start();
}

void ledOnOTA() {
    led.off();
    led.setDuration(10);
    led.setInterval(100);
    led.blink().start();
}