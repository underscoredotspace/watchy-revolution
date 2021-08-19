#pragma once

#include "Screen.h"

class WrappedTextScreen : public Screen {
 private:
  const char *text;

 public:
  WrappedTextScreen(const char *t, uint16_t bg = GxEPD_WHITE) : Screen(bg), text(t){};
  virtual void show() override;
  void setText(const char *t) { text = t; };
};