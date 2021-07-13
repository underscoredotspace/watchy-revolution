#ifndef SETUPWIFISCREEN_H
#define SETUPWIFISCREEN_H

#include "Screen.h"

class SetupWifiScreen : public Screen
{
public:
    SetupWifiScreen() : Screen("SetupWifiScreen"){};
    void show();
    void up();
    void down();
    void back();
    void menu();
};

extern SetupWifiScreen setupWifiScreen;
#endif