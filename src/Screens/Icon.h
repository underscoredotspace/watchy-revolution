#pragma once

#include "Adafruit_GFX.h"
#include "Screen.h"
#include "rle.h"

class Icon : public Screen {
 protected:
  const rle *rleImage;
  const char *label;
  const GFXfont *font;

 public:
  Icon(const rle *r, const char *l, const GFXfont *f)
      : Screen(), rleImage(r), label(l), font(f) {}
  void show() override {
    const uint16_t fgColor =
        (bgColor == GxEPD_WHITE ? GxEPD_BLACK : GxEPD_WHITE);
    Watchy::display.setFont(font);
    int16_t x1, y1;
    uint16_t w, h;
    drawRLEBitmap(Watchy::display, (200 - rleImage->w) / 2,
                  (200 - rleImage->h) / 2 / 2, rleImage, fgColor);
    Watchy::display.getTextBounds(label, 0, 0, &x1, &y1, &w, &h);
    // 170 should be 200 - baseline, but most font baselines are bogus
    Watchy::display.setCursor((200 - w) / 2, 170);
    Watchy::display.print(label);
  }
};
