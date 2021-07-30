#pragma once

#include "Screen.h"

class GetWeatherScreen : public Screen {
 public:
  GetWeatherScreen() : Screen() {}
  void show() override;
  void back() override;
};