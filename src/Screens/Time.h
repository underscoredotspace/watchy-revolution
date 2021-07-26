#pragma once

#include "Screen.h"

class Time : public Screen {
 public:
  Time(Screen *p = nullptr) : Screen(p) {}
  void show();
};
