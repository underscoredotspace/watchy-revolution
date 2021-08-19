#include "BuzzScreen.h"
#include "CarouselScreen.h"
#include "GetLocation.h"
#include "GetWeatherScreen.h"
#include "IconScreen.h"
#include "ImageScreen.h"
#include "MenuScreen.h"
#include "OptimaLTStd22pt7b.h"
#include "SetLocationScreen.h"
#include "SetTimeScreen.h"
#include "SetupWifiScreen.h"
#include "ShowBatteryScreen.h"
#include "ShowBluetoothScreen.h"
#include "ShowOrientationScreen.h"
#include "ShowStepsScreen.h"
#include "ShowWifiScreen.h"
#include "SyncTime.h"
#include "SyncTimeScreen.h"
#include "TimeScreen.h"
#include "UpdateFWScreen.h"
#include "Watchy.h"
#include "WatchyErrors.h"
#include "WeatherScreen.h"
#include "WrappedTextScreen.h"
#include "icons.h"

#include <time.h>

SetTimeScreen setTimeScreen;
SetupWifiScreen setupWifiScreen;
UpdateFWScreen updateFWScreen;
SyncTimeScreen syncTimeScreen;
SetLocationScreen setLocationScreen;
GetWeatherScreen getWeatherScreen;
BuzzScreen buzzScreen;

MenuItem menuItems[] = {{"Set Time", &setTimeScreen},
                        {"Setup WiFi", &setupWifiScreen},
                        {"Update Firmware", &updateFWScreen},
                        {"Sync Time", &syncTimeScreen},
                        {"Set Location", &setLocationScreen},
                        {"Get Weather", &getWeatherScreen},
                        {"Buzz", &buzzScreen}};

MenuScreen menu(menuItems, sizeof(menuItems) / sizeof(menuItems[0]));

TimeScreen timeScreen;
WeatherScreen weatherScreen;
IconScreen battery(&rle_battery, "battery", OptimaLTStd22pt7b);
IconScreen steps(&rle_steps, "steps", OptimaLTStd22pt7b);
IconScreen orientation(&rle_orientation, "orientation", OptimaLTStd22pt7b);
IconScreen bluetooth(&rle_bluetooth, "bluetooth", OptimaLTStd22pt7b);
IconScreen wifi(&rle_wifi, "wifi", OptimaLTStd22pt7b);
IconScreen settings(&rle_settings, "settings", OptimaLTStd22pt7b);
ImageScreen weather(cloud, 96, 96, "weather", OptimaLTStd22pt7b);
ShowBatteryScreen showBattery;
ShowBluetoothScreen showBluetooth;
ShowOrientationScreen showOrientation;
ShowStepsScreen showSteps;
ShowWifiScreen showWifi;
WrappedTextScreen wrappedTextScreen(
    "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Vivamus eu "
    "eleifend dui. Class aptent taciti sociosqu ad litora torquent per conubia "
    "nostra, per inceptos himenaeos. Etiam ante nulla, porta nec accumsan eu, "
    "viverra et ex.");

CarouselItem carouselItems[] = {{&timeScreen, nullptr},
                                {&weather, &weatherScreen},
                                {&battery, &showBattery},
                                {&steps, &showSteps},
                                {&orientation, &showOrientation},
                                {&wrappedTextScreen, nullptr},
                                {&bluetooth, &showBluetooth},
                                {&wifi, &showWifi},
                                {&settings, &menu}};

CarouselScreen carousel(carouselItems,
                        sizeof(carouselItems) / sizeof(carouselItems[0]));

void setup() {
  LOGD(); // fail if debugging macros not defined

  // initializing time and location can be a little tricky, because the
  // calls can fail for a number of reasons, but you don't want to just
  // keep trying because you can't know if the error is transient or
  // persistent. So whenever we wake up, try to sync the time and location
  // if they haven't ever been synced. If there is a persistent failure
  // this can drain your battery...
  if (Watchy_SyncTime::lastSyncTimeTS == 0) {
    Watchy_SyncTime::syncTime(Watchy_GetLocation::currentLocation.timezone);
  }
  if (Watchy_GetLocation::lastGetLocationTS == 0) {
    Watchy_GetLocation::getLocation();
  }
  if (Watchy::screen == nullptr) { Watchy::screen = &carousel; }
  Watchy::init();
}

void loop() {}  // this should never run, Watchy deep sleeps after init();