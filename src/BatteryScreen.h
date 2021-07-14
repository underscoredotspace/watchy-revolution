#ifndef BATTERYSCREEN_H
#define BATTERYSCREEN_H

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
#endif