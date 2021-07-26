#pragma once

#include "Screen.h"

class ShowBluetooth : public Screen {
 public:
  ShowBluetooth(Screen *p = nullptr) : Screen(p) {}
  void show() override;
};