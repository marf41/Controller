#include "ota.hpp"

String type;

void otaSetup() {
    // ArduinoOTA.setHostname("");
    ArduinoOTA.setPassword("controlpassword");
    ArduinoOTA.onStart([]() {
        type = (ArduinoOTA.getCommand() == U_FLASH) ?  "Sketch" : "Filesystem";
        // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
        Serial.println("Start updating: " + type);
        type = "OTA: " + type;
        oledOn(); clear(); drawString(type.c_str(), 4, 4); draw();
        ledOnOTA();
    });
    ArduinoOTA.onEnd([]() {
        Serial.println("\nEnd");
        delay(500);
        clear();
        esp_restart();
    });
    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
        uint8_t p = (progress * 100) / total;
        if (p > 100) { p = 100; }
        Serial.printf("Progress: %u%%.\n", p);
        clear();
        drawString(type.c_str(), 4, 4);
        drawProgressBar(4, 32, 120, 10, p);
        draw();
    });
    ArduinoOTA.onError([](ota_error_t error) {
        Serial.printf("Error[%u]: ", error);
        clear();
        if (error == OTA_AUTH_ERROR) {
            Serial.println("Auth Failed");
            drawString("ERROR: Auth failed.", 4, 60);
        } else if (error == OTA_BEGIN_ERROR) {
            Serial.println("Begin Failed");
            drawString("ERROR: Begin failed.", 4, 60);
        } else if (error == OTA_CONNECT_ERROR) {
            Serial.println("Connect Failed");
            drawString("ERROR: Connect failed.", 4, 60);
        } else if (error == OTA_RECEIVE_ERROR) {
            Serial.println("Receive Failed");
            drawString("ERROR: Receive failed.", 4, 60);
        } else if (error == OTA_END_ERROR) {
            Serial.println("End Failed");
            drawString("ERROR: End failed.", 4, 60);
        }
        draw();
    });
    ArduinoOTA.begin();
}

void otaLoop() { ArduinoOTA.handle(); }
