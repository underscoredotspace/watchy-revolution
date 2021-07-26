#pragma once

#include "Screen.h"

class Orientation : public Screen {
 public:
  Orientation(Screen *p = nullptr) : Screen(p) {}
  void show() override;
};
