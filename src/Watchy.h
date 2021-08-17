#pragma once

#include <Arduino.h>
#include <DS3232RTC.h>
#include <GxEPD2_BW.h>
#include <HTTPClient.h>
#include <WiFiManager.h>
#include <Wire.h>

#include "BLE.h"
#include "bma.h"
#include "config.h"

class Screen;

namespace Watchy {
extern DS3232RTC RTC;
extern GxEPD2_BW<GxEPD2_154_D67, GxEPD2_154_D67::HEIGHT> display;
extern tmElements_t currentTime;
extern Screen *screen;
void init(String datetime = "");
void deepSleep();
float getBatteryVoltage();

bool connectWiFi();

void showWatchFace(bool partialRefresh, Screen *s = screen);
void setScreen(Screen *s);
bool pollButtonsAndDispatch();  // returns true if button was pressed

// stored in RTC_DATA_ATTR
extern BMA423 sensor;
extern bool WIFI_CONFIGURED;
extern bool BLE_CONFIGURED;
};  // namespace Watchy
