#include "bootManager.h"
#include <Arduino.h>

BootManager bootManager;

void setup() {
    Serial.begin(115200);
    SPIFFS.begin();
    delay(50);
    WiFi.mode(WIFI_AP);
    WiFi.softAP("MultiUpload");
    IPAddress myIP = WiFi.softAPIP();
    DEBUG_MSG("AP IP address: ");
    DEBUG_MSG_LN(myIP);
    DEBUG_MSG_LN("MultiUploader Start");
    bootManager.startServer();
}

void loop() {}
