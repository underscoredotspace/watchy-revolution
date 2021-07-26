#include "Battery.h"
#include "Bluetooth.h"
#include "Carousel.h"
#include "Menu.h"
#include "Orientation.h"
#include "SetTime.h"
#include "SetLocation.h"
#include "Settings.h"
#include "SetupWifi.h"
#include "ShowBattery.h"
#include "ShowBluetooth.h"
#include "ShowOrientation.h"
#include "ShowSteps.h"
#include "ShowWifi.h"
#include "Steps.h"
#include "SyncTime.h"
#include "Time.h"
#include "UpdateFW.h"
#include "Watchy.h"
#include "Weather.h"
#include "Wifi.h"

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
Battery battery;
Steps steps;
Orientation orientation;
Bluetooth bluetooth;
Wifi wifi;
Settings settings;
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

Carousel carousel(carouselItems, sizeof(carouselItems) / sizeof(carouselItems[0]));

void setup() {
  Serial.begin(115200);
  Watchy::defaultScreen = &carousel;
  Watchy::init();
}

void loop() {}  // this should never run, Watchy deep sleeps after init();