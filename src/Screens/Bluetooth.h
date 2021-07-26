#pragma once

#include "Screen.h"

class Bluetooth : public Screen {
 public:
  Bluetooth(Screen *p = nullptr) : Screen(p) {}
  void show() override;
};