#pragma once

#include "Screen.h"

// default location is Melbourne Australia
constexpr const float DEFAULT_LOCATION_LATITUDE = 37.8136;
constexpr const float DEFAULT_LOCATION_LONGDITUDE = -144.9631;
constexpr const char * OPENWEATHERMAP_APIKEY = "f058fe1cad2afe8e2ddc5d063a64cecb"; //use your own API key :)
constexpr const char * OPENWEATHERMAP_URL = "http://api.openweathermap.org/data/2.5/weather?";
constexpr const char * TEMP_UNIT = "metric"; //use "imperial" for Fahrenheit"
const int WEATHER_UPDATE_INTERVAL = 30 * 60 * 1000; // 30 minutes in millis

typedef struct weatherData {
  int8_t temperature;
  int16_t weatherConditionCode;
} weatherData;

class GetWeatherScreen : public Screen {
 public:
  // this should probably live somewhere else (like in Watchy)
  // but can keep them here for now
  static weatherData currentWeather;
  static unsigned long lastWeatherTS; // millis()
  static weatherData getWeatherData();

 public:
  GetWeatherScreen() : Screen() {}
  void show() override;
  void back() override;
};