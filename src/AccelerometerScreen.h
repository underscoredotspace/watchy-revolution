#pragma once

#include "Screen.h"

class AccelerometerScreen : public Screen
{
public:
    AccelerometerScreen() : Screen("AccelerometerScreen"){};
    void show();
    void up();
    void down();
    void back();
    void menu();
};

extern AccelerometerScreen accelerometerScreen;