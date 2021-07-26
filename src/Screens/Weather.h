#pragma once

#include "Screen.h"

class Weather : public Screen {
 public:
  Weather(Screen *p = nullptr) : Screen(p) {}
  void show() override;
};
