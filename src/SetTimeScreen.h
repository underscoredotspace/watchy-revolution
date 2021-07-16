#pragma once

#include "Screen.h"

class SetTimeScreen : public Screen
{
public:
    SetTimeScreen() : Screen("SetTimeScreen"){};
    void show();
};

extern SetTimeScreen setTimeScreen;