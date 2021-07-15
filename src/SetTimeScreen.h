#ifndef SETTIMESCREEN_H
#define SETTIMESCREEN_H

#include "Screen.h"

class SetTimeScreen : public Screen
{
public:
    SetTimeScreen() : Screen("SetTimeScreen"){};
    void show();
};

extern SetTimeScreen setTimeScreen;
#endif