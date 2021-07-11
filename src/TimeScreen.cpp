#include "TimeScreen.h"
#include "Watchy.h"

void TimeScreen::show()
{
    w->RTC.read(w->currentTime);
    w->display.fillScreen(GxEPD_BLACK);
    w->display.setTextColor(GxEPD_WHITE);
    w->display.setFont(&DSEG7_Classic_Bold_53);
    w->display.setCursor(5, 53 + 60);
    w->display.printf("%02d:%02d", w->currentTime.Hour, w->currentTime.Minute);
}