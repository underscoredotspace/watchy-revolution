#include "TimeScreen.h"
#include "Watchy.h"

void TimeScreen::show()
{
    RTC.read(w->currentTime);
    w->display.fillScreen(GxEPD_BLACK);
    w->display.setTextColor(GxEPD_WHITE);
    w->display.setFont(&DSEG7_Classic_Bold_53);
    w->display.setCursor(5, 53 + 60);
    if (w->currentTime.Hour < 10)
    {
        w->display.print("0");
    }
    w->display.print(w->currentTime.Hour);
    w->display.print(":");
    if (w->currentTime.Minute < 10)
    {
        w->display.print("0");
    }
    w->display.println(w->currentTime.Minute);
}