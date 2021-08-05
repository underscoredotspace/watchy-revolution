#pragma once

#include "Screen.h"

class BuzzScreen : public Screen {
 public:
  BuzzScreen() : Screen() {}
  void show() override;
};
