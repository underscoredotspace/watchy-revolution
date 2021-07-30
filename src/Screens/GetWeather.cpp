#include "GetWeather.h"

#include <Arduino_JSON.h>
#include <Wifi.h>

#include "SetLocationScreen.h"
#include "Watchy.h"
#include "config.h"  // should be first

namespace Watchy_GetWeather {
RTC_DATA_ATTR weatherData currentWeather = {.temperature = 22,
                                            .weatherConditionCode = 800};
RTC_DATA_ATTR unsigned long lastWeatherTS = 0;

weatherData getWeather() {
  // only update if WEATHER_UPDATE_INTERVAL has elapsed i.e. 30 minutes
  if (now() - lastWeatherTS < WEATHER_UPDATE_INTERVAL) {
    // too soon to update, just re-use existing values. Not an error
    return currentWeather;
  }
  if (!Watchy::connectWiFi()) {
    // No WiFi, return RTC Temperature (this isn't actually useful...)
    uint8_t temperature = Watchy::RTC.temperature() / 4;  // celsius
    if (strcmp(TEMP_UNIT, "imperial") == 0) {
      temperature = temperature * 9. / 5. + 32.;  // fahrenheit
    }
    currentWeather.temperature = temperature;
    // we don't know, weatherConditionCode keep the last one
    return currentWeather;
  }

  // WiFi is connected Use Weather API for live data
  HTTPClient http;
  http.setConnectTimeout(5000);  // 5 second max timeout
  const unsigned int weatherQueryURLSize =
      strlen(OPENWEATHERMAP_URL) + strlen("?lat=") + 8 + strlen("&lon=") + 8 +
      strlen("&units=") + strlen(TEMP_UNIT) + strlen("&appid=") +
      strlen(OPENWEATHERMAP_APIKEY) + 1;
  char weatherQueryURL[weatherQueryURLSize];
  snprintf(weatherQueryURL, weatherQueryURLSize,
           "%s?lat=%.4f&lon=%.4f&units=%s&appid=%s", OPENWEATHERMAP_URL,
           SetLocationScreen::lat, SetLocationScreen::lon, TEMP_UNIT,
           OPENWEATHERMAP_APIKEY);
  http.begin(weatherQueryURL);
  int httpResponseCode = http.GET();
  if (httpResponseCode == 200) {
    String payload = http.getString();  // don't use String
    JSONVar responseObject = JSON.parse(payload);
    currentWeather.temperature = int(responseObject["main"]["temp"]);
    currentWeather.weatherConditionCode =
        int(responseObject["weather"][0]["id"]);
    lastWeatherTS = now();
  } else {
    LOGI("http response %d", httpResponseCode);
  }
  http.end();
  // turn off radios
  WiFi.mode(WIFI_OFF);
  btStop();
  return currentWeather;
}

}  // namespace Watchy_GetWeather