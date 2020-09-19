#include "main.hpp"

ModbusSerial mb;

EasyButton btnL(33, 80, true, false);
EasyButton btnR(32, 80, true, false);
EasyButton btnU(34, 80, true, false);
EasyButton btnD(36, 80, true, false);
EasyButton btnM(39, 80, true, false);

void onSent(const uint8_t *mac, esp_now_send_status_t status) {
    lastSend = status == ESP_NOW_SEND_SUCCESS;
}

void espNow() {
    if (esp_now_init() != ESP_OK) {
       return; 
    }
    esp_now_register_send_cb(onSent);
    esp_now_peer_info_t peerInfo;
    peerInfo.peer_addr
    peerInfo.channel = 0;
    peerInfo.encrypt = false;
    if (esp_now_add_peer(&peerInfo) != ESP_OK) {
        
    }
}

bool gui;

void onPressR() { tn = (tn == 1) ? 0 : tn++; }
void onPressL() { tn = (tn == 0) ? 11 : tn--; }
void onPressU() { target[tn] = (target[tn] == 0xF) ? 0 : target[tn]++; }
void onPressD() { target[tn] = (target[tn] == 0x0) ? 0xF : target[tn]--; }
void onPressM() { espNow(); }

uint8_t target[12] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
uint8_t tn = 0;

void setup() {
    gui = false;
    mb.config(&Serial, 9600, SERIAL_8N1);
    mb.setSlaveId(10);
    mb.addCoil(100);

    oledSetup(); clear();
    drawString("BOOTING: ", 4, 4); stepBoot();
    // drawProgressBar(4, 48, 120, 10, 0); draw();

    Serial.begin(115200);
    Serial.println("Booting...");

    drawBootStep("Analog");
    batterySetup();
    ledOnBoot();
    drawBootDone();

    // drawProgressBar(4, 32, 120, 10, 25); draw();

    drawBootStep("WIFI");
    WiFi.mode(WIFI_STA);
    WiFi.begin(wifissid, wifipass);
    if (WiFi.waitForConnectResult() != WL_CONNECTED) {
        Serial.println("Connection Failed!");
        drawBootFail();
        delay(3000);
        // ESP.restart();
    } else { drawBootDone(); }
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    drawBootStep("OTA");
    // drawProgressBar(4, 32, 120, 10, 50);
    otaSetup();
    drawBootDone();

    // drawProgressBar(4, 32, 120, 10, 75);
    drawString(WiFi.localIP().toString().c_str(), 4, 54); draw();

    btnL.begin(); btnR.begin(); btnU.begin(); btnD.begin(); btnM.begin();
    // bLeft.onPressedFor(1000, handleAction);
    btnL.onPressed(onPressL);
    btnR.onPressed(onPressR);
    btnU.onPressed(onPressU);
    btnD.onPressed(onPressD);
    btnM.onPressed(onPressM);

    drawBootStep("Server");
    serverSetup();
    drawBootDone();

    // drawProgressBar(4, 32, 120, 10, 100); draw();
    Serial.println("Done.");

    ledOnStandby();

    delay(3000);
    clear();
    oledOff();
}

void loop() {
    otaLoop();
    btnL.read(); btnR.read(); btnU.read(); btnD.read(); btnM.read();
    mb.task();
    if (gui) {
        // clear();
        drawString(serverInfo().c_str(), 4, 4);
        drawString(WiFi.localIP().toString().c_str(), 4, 54);
        drawString(mb.Coil(100) ? "Set" : "Unset", 4, 14);
        // drawString("OK", 4, 60);
        char battery[5] = "  0%";
        sprintf(battery, "%3d%%", calcPercentage(getVoltage()));
        drawString(battery, 106, 54);
        draw();
       // drawTest();
    }
    serverLoop();
    delay(30);
}
