#include "TimeScreen.h"

#include "Fonts/FreeSans24pt7b.h"
#include "Fonts/FreeSans9pt7b.h"
#include "Fonts/FreeSansBold24pt7b.h"
#include "SyncTime.h"

using namespace Watchy;

// inspired by http://rosettacode.org/wiki/Number_names#C.2B.2B
const char *smallNumbers[] = {"",        "one",       "two",      "three",
                              "four",    "five",      "six",      "seven",
                              "eight",   "nine",      "ten",      "eleven",
                              "twelve",  "thirteen",  "fourteen", "fifteen",
                              "sixteen", "seventeen", "eighteen", "nineteen"};
const char *decades[] = {"oh", nullptr, "twenty", "thirty", "forty", "fifty"};

void TimeScreen::show() {
  setenv("TZ", Watchy_SyncTime::tz, 1);
  tzset();
  tm t;
  time_t tt = now();
  localtime_r(&tt, &t);

  Watchy::display.fillScreen(bgColor);

  // hours
  display.setFont(&FreeSansBold24pt7b);
  // fudge height up a bit because baseline is too low
  display.setCursor(0, -5);
  display.printf("\n%s\n", smallNumbers[(t.tm_hour + 11) % 12 + 1]);  // 24->12

  // minutes
  display.setFont(&FreeSans24pt7b);
  if (t.tm_min == 0) {
    // 0: exactly on the hour
    if (t.tm_hour == 0) {
      display.println("midnight");
    } else if (t.tm_hour == 12) {
      display.println("noon");
    } else {
      display.println("o'clock");
    }
  } else if (10 <= t.tm_min && t.tm_min < 20) {
    // 10-19
    display.println(smallNumbers[t.tm_min]);
  } else if (t.tm_min <= 59) {
    // 1-9, 20-59
    display.println(decades[t.tm_min / 10]);
    display.println(smallNumbers[t.tm_min % 10]);
  }

  // date
  display.setCursor(0, 195);
  display.setFont(&FreeSans9pt7b);
  display.print(&t, "%a, %B %d %Y %Z");
}