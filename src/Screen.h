#pragma once

#include "Watchy.h"

// Abstract base class for screens. Each screen must provide a method for
// showing its face, and methods for handling up, down, back, and menu button
// presses. Default handlers set watchy screen to that screen.
class Screen {
 public:
  Screen* parent;
  uint16_t bgColor;
  Screen(Screen* p = nullptr, uint16_t bg = GxEPD_BLACK)
      : parent(p), bgColor(bg){};
  virtual void show() = 0;  // display this screen
  virtual void up() { DEBUG("Screen::up\n"); }
  virtual void down() { DEBUG("Screen::down\n"); }
  virtual void back() {
    DEBUG("Screen::back\n");
    Watchy::setScreen(parent);
  }
  virtual void menu() { DEBUG("Screen::menu\n"); }
};