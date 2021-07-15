#include "Watchy.h"
#include "MenuScreen.h"
#include "SetTimeScreen.h"

using namespace Watchy;

const int SET_HOUR = 0;
const int SET_MINUTE = 1;
const int SET_YEAR = 2;
const int SET_MONTH = 3;
const int SET_DAY = 4;

SetTimeScreen setTimeScreen;

void SetTimeScreen::show()
{
    const uint16_t fgColor = (screen->bgColor == GxEPD_WHITE ? GxEPD_BLACK : GxEPD_WHITE);

    tmElements_t currentTime;
    RTC.read(currentTime);

    int8_t minute = currentTime.Minute;
    int8_t hour = currentTime.Hour;
    int8_t day = currentTime.Day;
    int8_t month = currentTime.Month;
    int8_t year = currentTime.Year + YEAR_OFFSET - 2000;

    int8_t setIndex = SET_HOUR;

    int8_t blink = 0;

    pinMode(DOWN_BTN_PIN, INPUT);
    pinMode(UP_BTN_PIN, INPUT);
    pinMode(MENU_BTN_PIN, INPUT);
    pinMode(BACK_BTN_PIN, INPUT);

    while (1)
    {

        if (digitalRead(MENU_BTN_PIN) == 1)
        {
            setIndex++;
            if (setIndex > SET_DAY)
            {
                break;
            }
        }
        if (digitalRead(BACK_BTN_PIN) == 1)
        {
            if (setIndex != SET_HOUR)
            {
                setIndex--;
            }
        }

        blink = 1 - blink;

        if (digitalRead(DOWN_BTN_PIN) == 1)
        {
            blink = 1;
            switch (setIndex)
            {
            case SET_HOUR:
                hour == 23 ? (hour = 0) : hour++;
                break;
            case SET_MINUTE:
                minute == 59 ? (minute = 0) : minute++;
                break;
            case SET_YEAR:
                year == 99 ? (year = 20) : year++;
                break;
            case SET_MONTH:
                month == 12 ? (month = 1) : month++;
                break;
            case SET_DAY:
                day == 31 ? (day = 1) : day++;
                break;
            default:
                break;
            }
        }

        if (digitalRead(UP_BTN_PIN) == 1)
        {
            blink = 1;
            switch (setIndex)
            {
            case SET_HOUR:
                hour == 0 ? (hour = 23) : hour--;
                break;
            case SET_MINUTE:
                minute == 0 ? (minute = 59) : minute--;
                break;
            case SET_YEAR:
                year == 20 ? (year = 99) : year--;
                break;
            case SET_MONTH:
                month == 1 ? (month = 12) : month--;
                break;
            case SET_DAY:
                day == 1 ? (day = 31) : day--;
                break;
            default:
                break;
            }
        }

        display.fillScreen(screen->bgColor);
        display.setFont(&DSEG7_Classic_Bold_53);

        display.setCursor(5, 80);
        if (setIndex == SET_HOUR)
        { //blink hour digits
            display.setTextColor(blink ? fgColor : bgColor);
        }
        display.printf("%02d", hour);

        display.setTextColor(fgColor);
        display.print(":");

        display.setCursor(108, 80);
        if (setIndex == SET_MINUTE)
        { //blink minute digits
            display.setTextColor(blink ? fgColor : bgColor);
        }
        display.printf("%02d", minute);

        display.setTextColor(fgColor);

        display.setFont(&FreeMonoBold9pt7b);
        display.setCursor(45, 150);
        if (setIndex == SET_YEAR)
        { //blink minute digits
            display.setTextColor(blink ? fgColor : bgColor);
        }
        display.print(1970 + year);

        display.setTextColor(fgColor);
        display.print("/");

        if (setIndex == SET_MONTH)
        { //blink minute digits
            display.setTextColor(blink ? fgColor : bgColor);
        }
        display.printf("%02d", month);

        display.setTextColor(fgColor);
        display.print("/");

        if (setIndex == SET_DAY)
        { //blink minute digits
            display.setTextColor(blink ? fgColor : bgColor);
        }
        display.printf("%02d", day);
        display.display(true); //partial refresh
    }

    display.hibernate();

    const time_t FUDGE(10); //fudge factor to allow for upload time, etc. (seconds, YMMV)
    tmElements_t tm;
    tm.Month = month;
    tm.Day = day;
    tm.Year = year + 2000 - YEAR_OFFSET; //offset from 1970, since year is stored in uint8_t
    tm.Hour = hour;
    tm.Minute = minute;
    tm.Second = 0;

    time_t t = makeTime(tm) + FUDGE;
    RTC.set(t);
    setScreen(&menuScreen);
}
