#include "bootManager.h"
#include <Arduino.h>

BootManager bootManager(2);

void setup() {
    Serial.begin(115200);
    SPIFFS.begin();
    delay(50);
    DEBUG_MSG_LN("setup");
    WiFi.mode(WIFI_AP);
    WiFi.softAP("MultiUpload");
    IPAddress myIP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    DEBUG_MSG_LN(myIP);
    DEBUG_MSG_LN("startServer");
    bootManager.startServer();
}

unsigned long current = 0;
boolean isLedOn = false;

void loop() {
    if (millis() - current < 5000) {
        return;
    }
    current = millis();
    // DEBUG_MSG_LN("loop");
    // isLedOn ? digitalWrite(BOOT_MODE, LOW) : digitalWrite(BOOT_MODE, HIGH);
    // isLedOn = !isLedOn;
}
