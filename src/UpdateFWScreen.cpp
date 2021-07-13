#include "Watchy.h"
#include "Screen.h"
#include "MenuScreen.h"
#include "UpdateFWScreen.h"

using namespace Watchy;

UpdateFWScreen updateFWScreen;

void UpdateFWScreen::show()
{
    display.init(0, false); //_initial_refresh to false to prevent full update on init
    display.setFullWindow();
    display.fillScreen(GxEPD_BLACK);
    display.setFont(&FreeMonoBold9pt7b);
    display.setTextColor(GxEPD_WHITE);
    display.setCursor(0, 30);
    display.println("Please Visit");
    display.println("watchy.sqfmi.com");
    display.println("with a Bluetooth");
    display.println("enabled device");
    display.println(" ");
    display.println("Press menu button");
    display.println("again when ready");
    display.println(" ");
    display.println("Keep USB powered");
    display.display(false); //full refresh
    display.hibernate();
}
void UpdateFWScreen::up() { setScreen(&menuScreen); }
void UpdateFWScreen::down() { setScreen(&menuScreen); }
void UpdateFWScreen::back() { setScreen(&menuScreen); }
void UpdateFWScreen::menu() { Watchy::updateFWBegin(); }