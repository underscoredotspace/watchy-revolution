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

//weather api
constexpr const char * CITY_NAME = "NEW+YORK"; //if your city name has a space, replace with '+'
constexpr const char * COUNTRY_CODE = "US";
constexpr const char * OPENWEATHERMAP_APIKEY = "f058fe1cad2afe8e2ddc5d063a64cecb"; //use your own API key :)
constexpr const char * OPENWEATHERMAP_URL = "http://api.openweathermap.org/data/2.5/weather?q=";
constexpr const char * TEMP_UNIT = "metric"; //use "imperial" for Fahrenheit"
const int WEATHER_UPDATE_INTERVAL = 30; //minutes

//wifi
constexpr const char * WIFI_AP_SSID = "Watchy AP";
//set time
const int YEAR_OFFSET = 1970;

//BLE OTA
constexpr const char * BLE_DEVICE_NAME = "Watchy BLE OTA";
constexpr const char * WATCHFACE_NAME = "Watchy 7 Segment";
const int SOFTWARE_VERSION_MAJOR = 1;
const int SOFTWARE_VERSION_MINOR = 0;
const int SOFTWARE_VERSION_PATCH = 0;
const int HARDWARE_VERSION_MAJOR = 1;
const int HARDWARE_VERSION_MINOR = 0;

// debugging "#define DEBUGPORT Serial" for debugging output
#define DEBUGPORT Serial
#ifdef DEBUGPORT
#define DEBUG(...)      DEBUGPORT.printf(__VA_ARGS__)
#else
#define DEBUG(...)
#endif