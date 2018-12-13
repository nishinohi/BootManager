#include "bootManager.h"
#include <Arduino.h>

BootManager bootManager;

void setup() {
    Serial.begin(115200);
    SPIFFS.begin();
    delay(50);
    DEBUG_MSG_LN("MultiUploader Start");
    bootManager.startServer();
}

void loop() {}
