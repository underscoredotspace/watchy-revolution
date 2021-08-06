#pragma once

#include "Screen.h"

class SetTimeScreen : public Screen {
 private:
  static uint8_t setIndex;

 public:
  int8_t minute;
  int8_t hour;
  int8_t day;
  int8_t month;
  int8_t year;

 public:
  SetTimeScreen(uint16_t bg = GxEPD_WHITE) : Screen(bg) {}
  void show() override;
  void up() override;
  void down() override;
  void back() override;
  void menu() override;
};