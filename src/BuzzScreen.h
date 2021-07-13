#ifndef BUZZSCREEN_H
#define BUZZSCREEN_H

#include "Screen.h"

class BuzzScreen : public Screen
{
public:
    BuzzScreen() : Screen("BuzzScreen", GxEPD_BLACK){};
    void show();
    void up();
    void down();
    void back();
    void menu();
};

extern BuzzScreen buzzScreen;
#endif