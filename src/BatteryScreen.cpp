#include "Watchy.h"
#include "MenuScreen.h"
#include "BatteryScreen.h"

using namespace Watchy;

void BatteryScreen::show()
{
    display.init(0, false); //_initial_refresh to false to prevent full update on init
    display.setFullWindow();
    display.fillScreen(GxEPD_BLACK);
    display.setFont(&FreeMonoBold9pt7b);
    display.setTextColor(GxEPD_WHITE);
    display.setCursor(20, 30);
    display.println("Battery Voltage:");
    float voltage = getBatteryVoltage();
    display.setCursor(70, 80);
    display.print(voltage);
    display.println("V");
}

void BatteryScreen::up() { setScreen(&menuScreen); }
void BatteryScreen::down() { setScreen(&menuScreen); }
void BatteryScreen::back() { setScreen(&menuScreen); }
void BatteryScreen::menu() { setScreen(&menuScreen); }

BatteryScreen batteryScreen;