#pragma once

#include "Screen.h"

 class ShowOrientationScreen : public Screen {
 private:
  bool showing;
  void showMe();

 public:
  ShowOrientationScreen() : Screen() {}
  void show() override;
  void back() override;
};