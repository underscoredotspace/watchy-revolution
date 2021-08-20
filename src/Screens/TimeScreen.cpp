#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "SyncTime.h"
#include "TimeScreen.h"
#include "OverpassMono_Bold12pt7b.h"
#include "GetLocation.h"

using namespace Watchy;

void clamp(float *number, float min, float max) {
  if (*number < min) {
    *number = min;
  } else if (*number > max) {
    *number = max;
  }
}	

const char *bins[] = {
  "1111110001100011000111111",
  "0001100001000010000100001",
  "1111100001111111000011111",
  "1111100001011110000111111",
  "1000110001111110000100001",
  "1111110000111110000111111",
  "1111110000111111000111111",
  "1111100001000010000100001",
  "1111110001111111000111111",
  "1111110001111110000111111"
};

/*!
  @brief vibrate motor multiple times
  @param duration duration of each buzz in ms
  @param count number of buzzes
  @param space gap between each buzz
*/
void vibe(int duration, int count, int space) {
  pinMode(VIB_MOTOR_PIN, OUTPUT);
  for (int buzz = 0; buzz < count; buzz++) {
    digitalWrite(VIB_MOTOR_PIN, true);
    delay(duration);
    digitalWrite(VIB_MOTOR_PIN, false);
    delay(space);
  }
}

const int BLOCK_SIZE = 17;
const int DIGIT_SIZE = BLOCK_SIZE * 5;
const int PADDING = 5;
const int TIME_WIDTH = DIGIT_SIZE + 2 + DIGIT_SIZE;
const int CENTRE_PADDING = (DISPLAY_WIDTH - TIME_WIDTH) / 2;

void drawDigit(int digit, int digit_x, int digit_y) {
    int pos = 0;
    int startx;
    int starty;

    for (int ndx_y = 0; ndx_y < 5; ndx_y++) {
        for (int ndx_x = 0; ndx_x < 5; ndx_x++) {
            startx = CENTRE_PADDING + (ndx_x * BLOCK_SIZE) + digit_x;
            starty = ndx_y * BLOCK_SIZE + digit_y;

            if (bins[digit][pos] == '1') {
                display.fillRect(startx, starty, BLOCK_SIZE, BLOCK_SIZE, GxEPD_BLACK);
            }

            pos++;
        } //end for x
    } // end for y
}

void showTime(tm current_time) {
  int leftHour = floor(current_time.tm_hour / 10);
  int rightHour = current_time.tm_hour % 10;
  int leftMinute = floor(current_time.tm_min / 10);
  int rightMinute = current_time.tm_min % 10;

  drawDigit(leftHour, 0, 0);
  drawDigit(rightHour, DIGIT_SIZE + PADDING, 0);
  drawDigit(leftMinute, 0, DIGIT_SIZE + PADDING);
  drawDigit(rightMinute, DIGIT_SIZE + PADDING, DIGIT_SIZE + PADDING);
}

void showDate(tm current_time) {
  int16_t x1, y1;
  uint16_t w, h;
  String textstring;

  textstring = dayShortStr(current_time.tm_wday + 1);
  textstring += " ";
  textstring += current_time.tm_mday;
  textstring += " ";
  textstring += monthShortStr(current_time.tm_mon + 1);

  display.setFont(&OverpassMono_Bold12pt7b);
  display.getTextBounds(textstring, 0, 0, &x1, &y1, &w, &h);
  display.setCursor((DISPLAY_WIDTH - w - 1) / 2, DISPLAY_HEIGHT - 1);

  textstring.toUpperCase();
  display.print(textstring);
}

const float MAX_BATT = 4;
const float MIN_BATT =  3.4;

void showBatt() {
  display.setFont(&OverpassMono_Bold12pt7b);
  float batt_percentage = ((getBatteryVoltage() - MIN_BATT) * 100) / (MAX_BATT - MIN_BATT);
  clamp(&batt_percentage, 0, 100);

  display.setCursor(1, 17);
  display.printf("%0.0f%%", batt_percentage);
}

void TimeScreen::show() {
  tm current_time;
  time_t rtc_time = now();
  localtime_r(&rtc_time, &current_time);

  display.fillScreen(bgColor);
  display.setTextColor(GxEPD_BLACK);

  showTime(current_time);
  showDate(current_time);
  // showBatt();

  if(current_time.tm_min == 0) {
    display.refresh(true);
    if (current_time.tm_hour > 7 && current_time.tm_hour < 23) {
      vibe(150, 2, 100);
    } else if (current_time.tm_hour == 6) {
      // update the time at 0600
      Watchy_SyncTime::syncTime(Watchy_GetLocation::currentLocation.timezone);
    }
  }
}