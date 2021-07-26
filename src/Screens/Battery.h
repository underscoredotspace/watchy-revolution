#pragma once

#include "Screen.h"

class Battery : public Screen {
 public:
  Battery(Screen *p = nullptr) : Screen(p) {}
  void show() override;
};
