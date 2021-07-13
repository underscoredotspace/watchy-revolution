#include "Watchy.h"
#include "MenuScreen.h"
#include "BatteryScreen.h"

using namespace Watchy;

void BatteryScreen::show()
{
    display.setFont(&FreeMonoBold9pt7b);
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