#pragma once

//pins
#define SDA 21
#define SCL 22
#define ADC_PIN 33
#define RTC_PIN GPIO_NUM_27
#define CS 5
#define DC 10
#define RESET 9
#define BUSY 19
#define VIB_MOTOR_PIN 13
#define MENU_BTN_PIN 26
#define BACK_BTN_PIN 25
#define UP_BTN_PIN 32
#define DOWN_BTN_PIN 4
#define MENU_BTN_MASK GPIO_SEL_26
#define BACK_BTN_MASK GPIO_SEL_25
#define UP_BTN_MASK GPIO_SEL_32
#define DOWN_BTN_MASK GPIO_SEL_4
#define ACC_INT_MASK GPIO_SEL_14
#define BTN_PIN_MASK (MENU_BTN_MASK|BACK_BTN_MASK|UP_BTN_MASK|DOWN_BTN_MASK)
//display
const int DISPLAY_WIDTH = 200;
const int DISPLAY_HEIGHT = 200;

//wifi
constexpr const char * WIFI_AP_SSID = "Watchy AP";
//set time
const int YEAR_OFFSET = 1970;
constexpr const char *NTP_SERVER = "pool.ntp.org";

//BLE OTA
constexpr const char * BLE_DEVICE_NAME = "Watchy BLE OTA";
constexpr const char * WATCHFACE_NAME = "Watchy 7 Segment";
const int SOFTWARE_VERSION_MAJOR = 1;
const int SOFTWARE_VERSION_MINOR = 0;
const int SOFTWARE_VERSION_PATCH = 0;
const int HARDWARE_VERSION_MAJOR = 1;
const int HARDWARE_VERSION_MINOR = 0;

#include "wifi_config.h"

// debugging macros defined in DEBUG releases
#ifdef DEBUG
#define LOGE(...) ESP_LOGE(TAG, __VA_ARGS__)
#define LOGW(...) ESP_LOGW(TAG, __VA_ARGS__)
#define LOGI(...) ESP_LOGI(TAG, __VA_ARGS__)
#define LOGD(...) ESP_LOGD(TAG, __VA_ARGS__)
#define LOGV(...) ESP_LOGV(TAG, __VA_ARGS__)
#else
#define LOGE(...)
#define LOGW(...)
#define LOGI(...)
#define LOGD(...)
#define LOGV(...)
#endif