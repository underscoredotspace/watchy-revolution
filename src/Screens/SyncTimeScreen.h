#pragma once

#include "Screen.h"

class SyncTimeScreen : public Screen {
 public:
  SyncTimeScreen() : Screen() {}
  void show() override;
  void back() override;
};