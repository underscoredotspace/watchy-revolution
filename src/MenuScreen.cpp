#include "Watchy.h"
#include "MenuScreen.h"
#include "BatteryScreen.h"
#include "BuzzScreen.h"
#include "AccelerometerScreen.h"
#include "SetTimeScreen.h"
#include "SetupWifiScreen.h"
#include "UpdateFWScreen.h"

using namespace Watchy;

// consder using std::array
const char *const menuItems[] = {
    "Check Battery",
    "Vibrate Motor",
    "Show Accelerometer",
    "Set Time",
    "Setup WiFi",
    "Update Firmware"};

const unsigned int MENU_LENGTH = (sizeof(menuItems) / sizeof(menuItems[0]));

RTC_DATA_ATTR int MenuScreen::menuIndex;

void MenuScreen::show()
{
    DEBUG("MenuScreen::show index %d\n", menuIndex);
    display.fillScreen(GxEPD_BLACK);
    display.setFont(&FreeMonoBold9pt7b);

    for (int i = 0; i < MENU_LENGTH; i++)
    {
        int16_t yPos = 30 + (MENU_HEIGHT * i);
        display.setCursor(0, yPos);
        if (i == menuIndex)
        {
            int16_t x1, y1;
            uint16_t w, h;

            display.getTextBounds(menuItems[i], 0, yPos, &x1, &y1, &w, &h);
            display.fillRect(x1 - 1, y1 - 10, 200, h + 15, GxEPD_WHITE);
            display.setTextColor(GxEPD_BLACK);
            display.println(menuItems[i]);
        }
        else
        {
            display.setTextColor(GxEPD_WHITE);
            display.println(menuItems[i]);
        }
    }
}

void MenuScreen::menu()
{
    DEBUG("MenuScreen::menu\n");
    doMenu(menuIndex);
}

void MenuScreen::back()
{
    DEBUG("MenuScreen::back\n");
    RTC.alarm(ALARM_2); //resets the alarm flag in the RTC
    setScreen(watchFace);
}

void MenuScreen::up()
{
    DEBUG("MenuScreen::up\n");
    menuIndex--;
    if (menuIndex < 0)
    {
        menuIndex = MENU_LENGTH - 1;
    }
    show();
    display.display(true);
}

void MenuScreen::down()
{
    DEBUG("MenuScreen::down\n");
    menuIndex++;
    if (menuIndex > MENU_LENGTH - 1)
    {
        menuIndex = 0;
    }
    show();
    display.display(true);
}

MenuScreen menuScreen;

void MenuScreen::doMenu(int index)
{
    switch (index)
    {
    case 0:
        setScreen(&batteryScreen);
        break;
    case 1:
        setScreen(&buzzScreen);
        break;
    case 2:
        setScreen(&accelerometerScreen);
        break;
    case 3:
        setScreen(&setTimeScreen);
        break;
    case 4:
        setScreen(&setupWifiScreen);
        break;
    case 5:
        setScreen(&updateFWScreen);
        break;
    default:
        break;
    }
}