#include "TimeScreen.h"
#include "Watchy.h"

using namespace Watchy;

void TimeScreen::show()
{
    RTC.read(currentTime);
    display.fillScreen(GxEPD_BLACK);
    display.setTextColor(GxEPD_WHITE);
    display.setFont(&DSEG7_Classic_Bold_53);
    display.setCursor(5, 53 + 60);
    display.printf("%02d:%02d", currentTime.Hour, currentTime.Minute);
}