#include "Carousel.h"
#include "Fonts/FreeSans24pt7b.h"
#include "Icon.h"
#include "Menu.h"
#include "icons.h"
#include "SetLocation.h"
#include "SetTime.h"
#include "SetupWifi.h"
#include "ShowBattery.h"
#include "ShowBluetooth.h"
#include "ShowOrientation.h"
#include "ShowSteps.h"
#include "ShowWifi.h"
#include "SyncTime.h"
#include "Time.h"
#include "UpdateFW.h"
#include "Watchy.h"
#include "Weather.h"

SetTime setTimeScreen;
SetupWifi setupWifi;
UpdateFW updateFW;
SyncTime syncTime;
SetLocation setLocation;

MenuItem menuItems[] = {{"Set Time", &setTimeScreen},
                        {"Setup WiFi", &setupWifi},
                        {"Update Firmware", &updateFW},
                        {"Sync Time", &syncTime},
                        {"Set Location", &setLocation}};

Menu menu(menuItems, sizeof(menuItems) / sizeof(menuItems[0]));

Time timeScreen;
Weather weather;
Icon battery(&rle_battery, "battery", &FreeSans24pt7b);
Icon steps(&rle_steps, "steps", &FreeSans24pt7b);
Icon orientation(&rle_orientation, "orient", &FreeSans24pt7b);
Icon bluetooth(&rle_bluetooth, "bluetooth", &FreeSans24pt7b);
Icon wifi(&rle_wifi, "wifi", &FreeSans24pt7b);
Icon settings(&rle_settings, "settings", &FreeSans24pt7b);
ShowBattery showBattery;
ShowBluetooth showBluetooth;
ShowOrientation showOrientation;
ShowSteps showSteps;
ShowWifi showWifi;

CarouselItem carouselItems[] = {{&timeScreen, nullptr},
                                {&weather, nullptr},
                                {&battery, &showBattery},
                                {&steps, &showSteps},
                                {&orientation, &showOrientation},
                                {&bluetooth, &showBluetooth},
                                {&wifi, &showWifi},
                                {&settings, &menu}};

Carousel carousel(carouselItems,
                  sizeof(carouselItems) / sizeof(carouselItems[0]));

void setup() {
  Serial.begin(115200);
  Watchy::defaultScreen = &carousel;
  Watchy::init();
}

void loop() {}  // this should never run, Watchy deep sleeps after init();