#pragma once

#include "Watchy.h"

// Abstract base class for screens. Each screen must provide a method for
// showing its face, and methods for handling up, down, back, and menu button
// presses. Default handlers set watchy screen to that screen.
class Screen
{
public:
    const char* const name;
    uint16_t bgColor;
    Screen(const char *const n = "screen", uint16_t bg = GxEPD_BLACK)
        : name(n), bgColor(bg){};
    virtual void show() = 0; // display this screen
    virtual void up() { DEBUG("Screen::up\n"); }
    virtual void down() { DEBUG("Screen::down\n"); }
    virtual void back() { DEBUG("Screen::back\n"); }
    virtual void menu() { DEBUG("Screen::menu\n"); }
};