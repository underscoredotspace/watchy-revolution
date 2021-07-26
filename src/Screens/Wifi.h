#pragma once

#include "Screen.h"

class Wifi : public Screen {
 public:
  Wifi(Screen *p = nullptr) : Screen(p) {}
  void show() override;
};
