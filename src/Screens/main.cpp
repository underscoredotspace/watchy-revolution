#include "CarouselScreen.h"
#include "Fonts/FreeSans24pt7b.h"
#include "IconScreen.h"
#include "MenuScreen.h"
#include "SetLocationScreen.h"
#include "SetTimeScreen.h"
#include "SetupWifiScreen.h"
#include "ShowBatteryScreen.h"
#include "ShowBluetoothScreen.h"
#include "ShowOrientationScreen.h"
#include "ShowStepsScreen.h"
#include "ShowWifiScreen.h"
#include "SyncTimeScreen.h"
#include "TimeScreen.h"
#include "UpdateFWScreen.h"
#include "Watchy.h"
#include "WeatherScreen.h"
#include "icons.h"

SetTimeScreen setTimeScreen;
SetupWifiScreen setupWifi;
UpdateFWScreen updateFW;
SyncTimeScreen syncTime;
SetLocationScreen setLocation;

MenuItem menuItems[] = {{"Set Time", &setTimeScreen},
                        {"Setup WiFi", &setupWifi},
                        {"Update Firmware", &updateFW},
                        {"Sync Time", &syncTime},
                        {"Set Location", &setLocation}};

MenuScreen menu(menuItems, sizeof(menuItems) / sizeof(menuItems[0]));

TimeScreen timeScreen;
WeatherScreen weather;
IconScreen battery(&rle_battery, "battery", &FreeSans24pt7b);
IconScreen steps(&rle_steps, "steps", &FreeSans24pt7b);
IconScreen orientation(&rle_orientation, "orient", &FreeSans24pt7b);
IconScreen bluetooth(&rle_bluetooth, "bluetooth", &FreeSans24pt7b);
IconScreen wifi(&rle_wifi, "wifi", &FreeSans24pt7b);
IconScreen settings(&rle_settings, "settings", &FreeSans24pt7b);
ShowBatteryScreen showBattery;
ShowBluetoothScreen showBluetooth;
ShowOrientationScreen showOrientation;
ShowStepsScreen showSteps;
ShowWifiScreen showWifi;

CarouselItem carouselItems[] = {{&timeScreen, nullptr},
                                {&weather, nullptr},
                                {&battery, &showBattery},
                                {&steps, &showSteps},
                                {&orientation, &showOrientation},
                                {&bluetooth, &showBluetooth},
                                {&wifi, &showWifi},
                                {&settings, &menu}};

CarouselScreen carousel(carouselItems,
                        sizeof(carouselItems) / sizeof(carouselItems[0]));

void setup() {
#ifdef DEBUG
  Serial.begin(115200);
#endif
  LOGI(); // fail if debugging macros not defined
  Watchy::defaultScreen = &carousel;
  Watchy::init();
}

void loop() {}  // this should never run, Watchy deep sleeps after init();