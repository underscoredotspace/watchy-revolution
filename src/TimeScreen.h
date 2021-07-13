#ifndef TIMESCREEN_H
#define TIMESCREEN_H

#include "Screen.h"

class TimeScreen : public Screen
{
public:
    TimeScreen() : Screen("TimeScreen") {};
    virtual void show(); // display this screen
    virtual void menu();
};

extern TimeScreen timeScreen;
#endif