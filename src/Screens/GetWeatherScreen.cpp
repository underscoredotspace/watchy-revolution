#include "GetWeatherScreen.h"

#include <Arduino_JSON.h>

// #include "Adafruit_GFX.h"
#include "Fonts/FreeSans12pt7b.h"
#include "GetWeather.h"
#include "Watchy.h"

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
    auto weather = Watchy_GetWeather::getWeather();
    Watchy::display.printf("Temp %d, Weather %d", weather.temperature,
                           weather.weatherConditionCode);
  }
  Watchy::display.print("\npress back to exit");
}

void GetWeatherScreen::back() {
  getWeatherState = ready;
  Screen::back();
}
