#pragma once

#include "Screen.h"

class Settings : public Screen {
 public:
  Settings(Screen *p = nullptr) : Screen(p) {}
  void show() override;
};
