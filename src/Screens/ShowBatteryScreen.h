#pragma once

#include "Screen.h"

class ShowBatteryScreen : public Screen {
 public:
  ShowBatteryScreen() : Screen() {}
  void show() override;
};
