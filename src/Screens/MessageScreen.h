#pragma once

#include "Adafruit_GFX.h"
#include "OptimaLTStd12pt7b.h"
#include "Screen.h"

class MessageScreen : public Screen {
 private:
  static char buf[1024];
  static char *p;
  const GFXfont *font;

 public:
  MessageScreen(const GFXfont *f = OptimaLTStd12pt7b) : Screen(), font(f) {}
  void show() override;
  void back() override;
  static void clear();
  static bool empty() { return p==buf; };
  static int printf(const char* fmt...); // all these globals should be instances
  static size_t strftime(const tm *t, const char *fmt);
};