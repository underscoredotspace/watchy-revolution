#pragma once

#include "Screen.h"

 class ShowOrientation : public Screen {
 private:
  bool showing;
  void showMe();

 public:
  ShowOrientation(Screen *p = nullptr) : Screen(p) {}
  void show() override;
  void back() override;
};