#ifndef ACCELEROMETERSCREEN_H
#define ACCELEROMETERSCREEN_H

#include "Screen.h"

class AccelerometerScreen : public Screen
{
public:
    AccelerometerScreen() : Screen("AccelerometerScreen", GxEPD_BLACK){};
    void show();
    void up();
    void down();
    void back();
    void menu();
};

extern AccelerometerScreen accelerometerScreen;
#endif