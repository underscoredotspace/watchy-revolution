#pragma once

#include "Screen.h"

class WeatherScreen : public Screen {
 public:
  WeatherScreen() : Screen() {}
  void show() override;
};
