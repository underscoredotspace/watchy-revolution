#include "Time.h"

#include "Fonts/FreeSans24pt7b.h"
#include "Fonts/FreeSans9pt7b.h"
#include "Fonts/FreeSansBold24pt7b.h"
#include "SyncTime.h"

using namespace Watchy;

// inspired by http://rosettacode.org/wiki/Number_names#C.2B.2B
const char *smallNumbers[] = {"oh",      "one",       "two",      "three",
                              "four",    "five",      "six",      "seven",
                              "eight",   "nine",      "ten",      "eleven",
                              "twelve",  "thirteen",  "fourteen", "fifteen",
                              "sixteen", "seventeen", "eighteen", "nineteen"};
const char *decades[] = {"oh", nullptr, "twenty", "thirty", "forty", "fifty"};

void Time::show() {
  tmElements_t currentTime;
  Watchy::RTC.read(currentTime);
  time_t tt = makeTime(currentTime);

  setenv("TZ", SyncTime::tz, 1);
  tzset();
  tm t;
  localtime_r(&tt, &t);

  // hour, just one line
  display.setFont(&FreeSansBold24pt7b);
  display.setCursor(0, -5);
  display.printf("\n%s\n", smallNumbers[(t.tm_hour + 11) % 12 + 1]);  // 24->12

  // minutes, can be up to two lines
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
    display.println(smallNumbers[t.tm_min]);
  } else if (t.tm_min <= 59) {
    display.println(decades[t.tm_min / 10]);
    if (t.tm_min % 10 != 0) {
    display.println(smallNumbers[t.tm_min % 10]);
    }
  }

  // date
  display.setCursor(0, 195);
  display.setFont(&FreeSans9pt7b);
  display.print(&t, "%a, %B %d %Y %Z");
}