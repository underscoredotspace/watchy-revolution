#pragma once

#include "Screen.h"

class ShowWifiScreen : public Screen {
 public:
  ShowWifiScreen() : Screen() {}
  void show() override;
};
