#pragma once

#include "Screen.h"

class ShowBattery : public Screen {
 public:
  ShowBattery(Screen *p = nullptr) : Screen(p) {}
  void show() override;
};
