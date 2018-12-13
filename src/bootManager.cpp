#include "BootManager.h"

/**
 * Server 設定
 */
void BootManager::setupServer() {
    _server.on("/onPowerSupply", HTTP_POST,
               std::bind(&BootManager::onPowerSupply, this, std::placeholders::_1));
    _server.serveStatic("/", SPIFFS, "/").setDefaultFile("index.html");
    _server.onNotFound([](AsyncWebServerRequest *request) { request->send(404); });
}

// 指定された GPIO に接続しているモジュールに電源を供給する
void BootManager::onPowerSupply(AsyncWebServerRequest *request) {
    DEBUG_MSG_LN("onPowerSupply");
    String powerPin = request->arg(POWER_KEY);
    if (powerPin == NULL || powerPin.length() == 0) {
        request->send(500);
        return;
    }
    // 利用可能な GPIO のピン番号以上のピンを指定された場合
    if (powerPin.toInt() >= _powerPinNum) {
        request->send(500);
        return;
    }
    // 起動モード
    String bootMode = request->arg(BOOT_MODE_KEY);
    if (bootMode == NULL || bootMode.length() == 0) {
        request->send(500);
        return;
    }
    powerSupplyModule(powerPin.toInt());
    resetModule(bootMode.toInt() == 1 ? true : false);
    request->send(200);
}

// 指定したピン番号のモジュールに電源を供給
void BootManager::powerSupplyModule(uint8_t powerPin) {
    DEBUG_MSG_LN("powerSupplyModule");
    changePinState(powerPin, HIGH);
    _currentSupplyPin = powerPin;
}

// モジュールをリセット
// uploadMode:false -> normal mode(default)
// uploadMode:true  -> upload mode
void BootManager::resetModule(bool uploadMode) {
    DEBUG_MSG_LN("resetModule");
    DEBUG_MSG_LN(uploadMode ? "upload mode" : "normal mode");
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
// ex) changePinState(0, true) -> POWER_0:HIGH, POWER_1～11:LOW
void BootManager::changePinState(uint8_t powerPin, bool state) {
    DEBUG_MSG_LN("changePinState");
    for (int ii = 0; ii < _powerPinNum; ++ii) {
        uint8_t output = ii == powerPin ? (state ? HIGH : LOW) : (state ? LOW : HIGH);
        DEBUG_MSG_F("GPIO_%d:%d\n", _powerPins[ii], output);
        digitalWrite(_powerPins[ii], output);
    }
}

// GPIO 初期化
void BootManager::initPinMode() {
    DEBUG_MSG_LN("initPinMode");
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
    // LED
    pinMode(LED, OUTPUT);
    digitalWrite(LED, HIGH);
}