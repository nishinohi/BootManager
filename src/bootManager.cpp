#include "BootManager.h"

/**
 * Server 設定
 */
void BootManager::setupServer() {
    _server.on("/onSearchModule", HTTP_POST,
               std::bind(&BootManager::onSearchModule, this, std::placeholders::_1));
    _server.on("/onUploadEnd", HTTP_GET,
               std::bind(&BootManager::onUploadEnd, this, std::placeholders::_1));
    _server.on("/onModuleShift", HTTP_GET,
               std::bind(&BootManager::onModuleShift, this, std::placeholders::_1));
    _server.serveStatic("/", SPIFFS, "/").setDefaultFile("index.html");
    _server.onNotFound([](AsyncWebServerRequest *request) { request->send(404); });
}

void BootManager::onSearchModule(AsyncWebServerRequest *request) {
    DEBUG_MSG_LN("onSearchModule");
    String powerPin = request->arg(POWER_KEY);
    if (powerPin == NULL || powerPin.length() == 0) {
        request->send(500);
        return;
    }
    shitWakeupModule();
    request->send(200);
}

void BootManager::onUploadEnd(AsyncWebServerRequest *request) {
    DEBUG_MSG_LN("onUploadEnd");
    resetModule(true);
    request->send(200);
}

void BootManager::onModuleShift(AsyncWebServerRequest *request) {
    DEBUG_MSG_LN("onModuleShift");
    shitWakeupModule();
    request->send(200);
}

void BootManager::wakeupModule(uint8_t powerPin) {
    DEBUG_MSG("wakeupModule");
    DEBUG_MSG_LN(powerPin);
    if (_moduleId != 0) {
        digitalWrite(_powerPins[_moduleId - 1], HIGH);
    }
    digitalWrite(_powerPins[_moduleId], LOW);
    ++_moduleId;
}

void BootManager::shitWakeupModule() {
    DEBUG_MSG("shitWakeupModule");
    DEBUG_MSG_LN(_moduleId);
    if (_moduleId != 0) {
        digitalWrite(_powerPins[_moduleId - 1], HIGH);
    }
    digitalWrite(_powerPins[_moduleId], LOW);
    ++_moduleId;
}

void BootManager::resetModule(bool uploadMode) {
    digitalWrite(BOOT_MODE, uploadMode ? LOW : HIGH);
    delay(100);
    digitalWrite(RESET, LOW);
    delay(100);
    digitalWrite(RESET, HIGH);
    delay(100);
    digitalWrite(BOOT_MODE, HIGH);
}

// 任意のピンの出力状態を HIGH or LOW にし
// その他のピンの出力状態を反対にする
void BootManager::changePinState(uint8_t powerPin, bool state) {
    for (int ii = 0; ii < _powerPinNum; ++ii) {
        uint8_t output = ii == powerPin ? (state ? HIGH : LOW) : (state ? LOW : HIGH);
        digitalWrite(_powerPins[ii], output);
    }
}

void BootManager::initPinMode() {
    // POWER PIN
    for (int ii = 0; ii < _powerPinNum; ++ii) {
        pinMode(_powerPins[ii], OUTPUT);
    }
    for (int ii = 0; ii < _powerPinNum; ++ii) {
        digitalWrite(_powerPins[ii], LOW);
    }
    // BOOT MODE(default is run mode)
    pinMode(BOOT_MODE, OUTPUT);
    digitalWrite(BOOT_MODE, HIGH);
    // RESET
    pinMode(RESET, OUTPUT);
    digitalWrite(RESET, HIGH);
}