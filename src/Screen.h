#ifndef SCREEN_H
#define SCREEN_H

// Abstract base class for screens. Each screen must provide a method for
// showing its face, and methods for handling up, down, back, and menu button
// presses. Default handlers set watchy screen to that screen.
class Screen
{
public:
    virtual void show() = 0; // display this screen
};
#endif