#ifndef SCREEN_H
#define SCREEN_H

#include "Watchy.h"

// Abstract base class for screens. Each screen must provide a method for
// showing its face, and methods for handling up, down, back, and menu button
// presses. Default handlers set watchy screen to that screen.
class Screen
{
protected:
    Screen *_up, *_down, *_back, *_menu;

public:
    virtual void show() = 0; // display this screen
    virtual void up()
    {
        DEBUG("up\n");
        Watchy::setScreen(_up);
    }
    virtual void down()
    {
        DEBUG("down\n");
        Watchy::setScreen(_down);
    }
    virtual void back()
    {
        DEBUG("back\n");
        Watchy::setScreen(_back);
    }
    virtual void menu()
    {
        DEBUG("menu\n");
        Watchy::setScreen(_menu);
    }
    void setUp(Screen *s) { _up = s; }
    void setDown(Screen *s) { _down = s; }
    void setBack(Screen *s) { _back = s; }
    void setMenu(Screen *s) { _menu = s; }
};
#endif