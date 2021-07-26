#pragma once

#include "Screen.h"

class Steps : public Screen {
 public:
  Steps(Screen *p = nullptr) : Screen(p) {}
  void show() override;
};
