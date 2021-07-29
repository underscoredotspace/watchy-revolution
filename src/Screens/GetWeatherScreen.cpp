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
  syncFailed,
  syncSucceeded,
  numGetWeatherStates
} getWeatherState = ready;

const char *getWeatherMsgs[numGetWeatherStates] = {
    "\nready", "\nsynchronizing", "\nwifi connect failed", "\nsync failed",
    "\nsuccess"};

weatherData GetWeatherScreen::getWeatherData() {
  // only update if WEATHER_UPDATE_INTERVAL has elapsed i.e. 30 minutes
  if (millis() - GetWeatherScreen::lastWeatherTS >= WEATHER_UPDATE_INTERVAL) {
    if (Watchy::connectWiFi()) {  // Use Weather API for live data if WiFi is connected
      HTTPClient http;
      http.setConnectTimeout(3000);  // 3 second max timeout
      const unsigned int weatherQueryURLSize =
          strlen(OPENWEATHERMAP_URL) + strlen("?lat=") + 8 + strlen("&lon=") +
          8 + strlen("&units=") + strlen(TEMP_UNIT) + strlen("&appid=") +
          strlen(OPENWEATHERMAP_APIKEY) + 1;
      char weatherQueryURL[weatherQueryURLSize];
      snprintf(weatherQueryURL, weatherQueryURLSize,
               "%s?lat=%8.4f&lon=%8.4f&units=%s&appid=%s", OPENWEATHERMAP_URL,
               SetLocationScreen::lat, SetLocationScreen::lon, TEMP_UNIT,
               OPENWEATHERMAP_APIKEY);
      http.begin(weatherQueryURL);
      int httpResponseCode = http.GET();
      if (httpResponseCode == 200) {
        String payload = http.getString();
        JSONVar responseObject = JSON.parse(payload);
        GetWeatherScreen::currentWeather.temperature = int(responseObject["main"]["temp"]);
        GetWeatherScreen::currentWeather.weatherConditionCode =
            int(responseObject["weather"][0]["id"]);
        GetWeatherScreen::lastWeatherTS = millis();
      } else {
        // http error
      }
      http.end();
      // turn off radios
      WiFi.mode(WIFI_OFF);
      btStop();
    } else {  // No WiFi, use RTC Temperature
      uint8_t temperature = Watchy::RTC.temperature() / 4;  // celsius
      if (strcmp(TEMP_UNIT, "imperial") == 0) {
        temperature = temperature * 9. / 5. + 32.;  // fahrenheit
      }
      GetWeatherScreen::currentWeather.temperature = temperature;
      GetWeatherScreen::currentWeather.weatherConditionCode = 800;
    }
  }
  return GetWeatherScreen::currentWeather;
}

void GetWeatherScreen::show() {
  Watchy::display.setFont(&FreeSans12pt7b);
  if (getWeatherState != ready) {
    Watchy::display.print(getWeatherMsgs[getWeatherState]);
  } else {
    time_t tt;
    time(&tt);
    getWeatherState = waitingForSync;
    Watchy::display.print(getWeatherMsgs[getWeatherState]);
    Watchy::display.display(true);
    Watchy::display.fillScreen(bgColor);
    Watchy::display.setCursor(0, 0);

    if (!Watchy::connectWiFi()) {
      getWeatherState = wifiFailed;
      Watchy::display.print(getWeatherMsgs[getWeatherState]);
    } else {
      getWeatherData();
    }
  }
  Watchy::display.print("\npress back to exit");
}

void GetWeatherScreen::back() {
  getWeatherState = ready;
  Screen::back();
}

// RTC does not know about TZ
// so DST has to be in app code
// so RTC must store UTC
// RTC computes leap year so you want year to be correct
// according to how the RTC represents it