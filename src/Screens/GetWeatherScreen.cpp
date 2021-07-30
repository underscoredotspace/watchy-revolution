#include "GetWeatherScreen.h"

#include "config.h"  // should be first

#include <Arduino_JSON.h>
#include <Wifi.h>

#include "Adafruit_GFX.h"
#include "Fonts/FreeSans12pt7b.h"
#include "SetLocationScreen.h"
#include "Watchy.h"
#include "icons.h"
#include "time.h"

RTC_DATA_ATTR weatherData GetWeatherScreen::currentWeather;
RTC_DATA_ATTR unsigned long GetWeatherScreen::lastWeatherTS = 0;

RTC_DATA_ATTR enum GetWeatherState {
  ready,
  waitingForSync,
  wifiFailed,
  httpGetFailed,
  getWeatherSuccess,
  numGetWeatherStates
} getWeatherState = ready;

const char *getWeatherMsgs[numGetWeatherStates] = {
    "\nready", "\nsynchronizing", "\nwifi connect failed", "\nhttp get failed",
    "\nsuccess"};

weatherData GetWeatherScreen::getWeatherData() {
  // only update if WEATHER_UPDATE_INTERVAL has elapsed i.e. 30 minutes
  if (now() - lastWeatherTS < WEATHER_UPDATE_INTERVAL) {
    // too soon to update, just re-use existing values. Not an error
  } else {
    if (Watchy::connectWiFi()) {  // Use Weather API for live data if WiFi is
                                  // connected
      HTTPClient http;
      http.setConnectTimeout(3000);  // 3 second max timeout
      const unsigned int weatherQueryURLSize =
          strlen(OPENWEATHERMAP_URL) + strlen("?lat=") + 8 + strlen("&lon=") +
          8 + strlen("&units=") + strlen(TEMP_UNIT) + strlen("&appid=") +
          strlen(OPENWEATHERMAP_APIKEY) + 1;
      char weatherQueryURL[weatherQueryURLSize];
      snprintf(weatherQueryURL, weatherQueryURLSize,
               "%s?lat=%.4f&lon=%.4f&units=%s&appid=%s", OPENWEATHERMAP_URL,
               SetLocationScreen::lat, SetLocationScreen::lon, TEMP_UNIT,
               OPENWEATHERMAP_APIKEY);
      http.begin(weatherQueryURL);
      int httpResponseCode = http.GET();
      if (httpResponseCode == 200) {
        String payload = http.getString();
        JSONVar responseObject = JSON.parse(payload);
        currentWeather.temperature = int(responseObject["main"]["temp"]);
        currentWeather.weatherConditionCode =
            int(responseObject["weather"][0]["id"]);
        lastWeatherTS = now();
        getWeatherState = getWeatherSuccess;
        Watchy::display.print(getWeatherMsgs[getWeatherState]);
      } else {
        getWeatherState = httpGetFailed;
        Watchy::display.print(getWeatherMsgs[getWeatherState]);
      }
      http.end();
      // turn off radios
      WiFi.mode(WIFI_OFF);
      btStop();
    } else {  // No WiFi, use RTC Temperature
      getWeatherState = wifiFailed;
      Watchy::display.print(getWeatherMsgs[getWeatherState]);

      uint8_t temperature = Watchy::RTC.temperature() / 4;  // celsius
      if (strcmp(TEMP_UNIT, "imperial") == 0) {
        temperature = temperature * 9. / 5. + 32.;  // fahrenheit
      }
      currentWeather.temperature = temperature;
      currentWeather.weatherConditionCode = 800;
    }
  }
  return currentWeather;
}

void GetWeatherScreen::show() {
  Watchy::display.fillScreen(bgColor);
  Watchy::display.setFont(&FreeSans12pt7b);
  if (getWeatherState != ready) {
    Watchy::display.print(getWeatherMsgs[getWeatherState]);
  } else {
    getWeatherState = waitingForSync;
    Watchy::display.print(getWeatherMsgs[getWeatherState]);
    Watchy::display.display(true);
    Watchy::display.fillScreen(bgColor);
    Watchy::display.setCursor(0, 0);
    getWeatherData();
  }
  Watchy::display.print("\npress back to exit");
}

void GetWeatherScreen::back() {
  getWeatherState = ready;
  Screen::back();
}
