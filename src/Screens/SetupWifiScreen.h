#pragma once

#include "Screen.h"

class SetupWifiScreen : public Screen {
 public:
  SetupWifiScreen() : Screen() {}
  void show() override;
  void back() override;
};
