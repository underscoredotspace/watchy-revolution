#pragma once

#include "Screen.h"

class SetupWifi : public Screen {
 public:
  SetupWifi(Screen *p = nullptr) : Screen(p) {}
  void show() override;
  void back() override;
};
