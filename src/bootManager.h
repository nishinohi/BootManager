#ifndef INCLUDE_GUARD_SERVER
#define INCLUDE_GUARD_SERVER

#include "ESPAsyncWebServer.h"
#include <SPIFFS.h>

#define DEBUG_ESP_PORT Serial
#ifdef DEBUG_ESP_PORT
#define DEBUG_MSG_LN(...) DEBUG_ESP_PORT.println(__VA_ARGS__)
#define DEBUG_MSG_F(...) DEBUG_ESP_PORT.printf(__VA_ARGS__)
#define DEBUG_MSG(...) DEBUG_ESP_PORT.print(__VA_ARGS__)
#else
#define DEBUG_MSG_LN(...)
#define DEBUG_MSG_F(...)
#define DEBUG_MSG(...)
#endif

#define POWER_0 16
#define POWER_1 17
#define POWER_2 18
#define POWER_3 19
#define POWER_4 21
#define POWER_5 22
#define POWER_6 23
#define POWER_7 25
#define POWER_8 26
#define POWER_9 27
#define POWER_10 32
#define POWER_11 33
#define PIN_NUM 12

#define LED 4
#define BOOT_MODE 13
#define RESET 14

#define POWER_KEY "powerKey"

class BootManager {
  private:
    AsyncWebServer _server = AsyncWebServer(80);
    uint8_t _currentModuleId = 0;
    uint8_t _moduleNum = 0;
    uint8_t _moduleId = 0;
    uint8_t _powerPins[PIN_NUM] = {POWER_0, POWER_1, POWER_2, POWER_3, POWER_4,  POWER_5,
                                   POWER_6, POWER_7, POWER_8, POWER_9, POWER_10, POWER_11};
    uint8_t _powerPinNum = 0;

  public:
    BootManager(uint8_t moduleNum) {
        initPinMode();
        _moduleNum = moduleNum;
        setupServer();
        _powerPinNum = sizeof(_powerPins) / sizeof(uint8_t);
    };

    void startServer() { _server.begin(); }
    void onSearchModule(AsyncWebServerRequest *request);
    void onUploadEnd(AsyncWebServerRequest *request);
    void onModuleShift(AsyncWebServerRequest *request);
    // モジュール起動管理
    void resetModule(bool uploadMode = false);
    void wakeupModule(uint8_t powerPin);
    void shitWakeupModule();

  private:
    void initPinMode();
    void changePinState(uint8_t powerPin, bool state);
    void setupServer();
};

#endif // INCLUDE_GUARD_SERVER
