#pragma once

#include "Screen.h"

class BatteryScreen : public Screen
{
public:
    BatteryScreen() : Screen("BatteryScreen"){};
    void show();
    void up();
    void down();
    void back();
    void menu();
};

extern BatteryScreen batteryScreen;