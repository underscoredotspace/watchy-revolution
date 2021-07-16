#pragma once

#include "Screen.h"

class BuzzScreen : public Screen
{
public:
    BuzzScreen() : Screen("BuzzScreen"){};
    void show();
    void up();
    void down();
    void back();
    void menu();
};

extern BuzzScreen buzzScreen;