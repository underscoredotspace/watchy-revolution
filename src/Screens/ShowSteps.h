#pragma once

#include "Screen.h"

class ShowSteps : public Screen {
 public:
  ShowSteps(Screen *p = nullptr) : Screen(p) {}
  void show() override;
};
