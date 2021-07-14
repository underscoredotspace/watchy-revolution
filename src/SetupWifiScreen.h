#ifndef SETUPWIFISCREEN_H
#define SETUPWIFISCREEN_H

#include "Screen.h"

class SetupWifiScreen : public Screen
{
public:
    SetupWifiScreen() : Screen("SetupWifiScreen"){};
    void show();
};

extern SetupWifiScreen setupWifiScreen;
#endif