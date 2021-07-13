#ifndef UPDATEFWSCREEN_H
#define UPDATEFWSCREEN_H

#include "Screen.h"

class UpdateFWScreen : public Screen
{
public:
    UpdateFWScreen() : Screen("UpdateFWScreen", GxEPD_BLACK){};
    void show();
    void up();
    void down();
    void back();
    void menu();
};

extern UpdateFWScreen updateFWScreen;
#endif