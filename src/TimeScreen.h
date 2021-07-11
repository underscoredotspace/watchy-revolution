#ifndef TIMESCREEN_H
#define TIMESCREEN_H

#include "Screen.h"

class Watchy;

// Abstract base class for screens. Each screen must provide a method for
// showing its face, and methods for handling up, down, back, and menu button
// presses. Default handlers set watchy screen to that screen.
class TimeScreen : public Screen
{
public:
    TimeScreen(Watchy *_w) : Screen(_w) {};
    virtual void show(); // display this screen
};
#endif