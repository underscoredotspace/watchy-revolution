#pragma once

#include "Screen.h"

class UpdateFWScreen : public Screen {
 public:
  UpdateFWScreen() : Screen() {}
  void show() override;
  void menu() override;
};