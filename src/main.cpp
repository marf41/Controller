#include "main.hpp"

EasyButton button(33, 80, true, false);

void onPressR() {
    Serial.println("Button!");
}

void setup() {
    oledSetup();
    Serial.begin(115200);
    Serial.println("Booting...");
    WiFi.mode(WIFI_STA);
    WiFi.begin(wifissid, wifipass);
    if (WiFi.waitForConnectResult() != WL_CONNECTED) {
        Serial.println("Connection Failed! Rebooting...");
        delay(5000);
        ESP.restart();
    }
    otaSetup();
    Serial.println("Ready.");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    button.begin();
    // button.onPressedFor(1000, handleAction);
    button.onPressed(onPressR);

    Serial.println("Done.");
}

void loop() {
    otaLoop();
    button.read();
    /*
    clear();
    drawString(WiFi.localIP().toString().c_str(), 4, 60);
    draw();
    delay(100);
    */
}
