#pragma once

#include "Screen.h"

class ShowBluetoothScreen : public Screen {
 public:
  ShowBluetoothScreen() : Screen() {}
  void show() override;
};