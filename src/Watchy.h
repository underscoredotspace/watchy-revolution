#ifndef WATCHY_H
#define WATCHY_H

#include <Arduino.h>
#include <WiFiManager.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>
#include <DS3232RTC.h>
#include <GxEPD2_BW.h>
#include <Wire.h>
#include <Fonts/FreeMonoBold9pt7b.h>
#include "DSEG7_Classic_Bold_53.h"
#include "BLE.h"
#include "bma.h"
#include "config.h"

class Screen;

namespace Watchy {
    typedef struct weatherData{
        int8_t temperature;
        int16_t weatherConditionCode;
    }weatherData;

    extern DS3232RTC RTC;
    extern GxEPD2_BW<GxEPD2_154_D67, GxEPD2_154_D67::HEIGHT> display;
    extern tmElements_t currentTime;
    extern Screen *screen;
    void init(String datetime = "");
    void deepSleep();
    float getBatteryVoltage();

    void handleButtonPress();
    void showMenu(byte menuIndex, bool partialRefresh);
    void showFastMenu(byte menuIndex);
    void showBattery();
    void showBuzz();
    void showAccelerometer();
    void showUpdateFW();
    void setTime();
    void setupWifi();
    bool connectWiFi();
    weatherData getWeatherData();

    void showWatchFace(bool partialRefresh);
    void setScreen(Screen *s);
    bool pollButtonsAndDispatch(); // returns true if button was pressed

    // stored in RTC_DATA_ATTR
    extern int guiState;
    extern int menuIndex;
    extern BMA423 sensor;
    extern bool WIFI_CONFIGURED;
    extern bool BLE_CONFIGURED;
};

#endif