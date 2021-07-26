#pragma once

#include "Screen.h"

class ShowWifi : public Screen {
 public:
  ShowWifi(Screen *p = nullptr) : Screen(p) {}
  void show() override;
};
