#pragma once

#include "Screen.h"

class UpdateFW : public Screen {
 public:
  UpdateFW(Screen *p = nullptr) : Screen(p) {}
  void show() override;
  void menu() override;
};