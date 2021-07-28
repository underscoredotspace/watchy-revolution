#pragma once

#include "Adafruit_GFX.h"
#include "Screen.h"
#include "rle.h"

class IconScreen : public Screen {
 protected:
  const rle *rleImage;
  const char *label;
  const GFXfont *font;

 public:
  IconScreen(const rle *r, const char *l, const GFXfont *f)
      : Screen(), rleImage(r), label(l), font(f) {}
  void show() override {
    const uint16_t fgColor =
        (bgColor == GxEPD_WHITE ? GxEPD_BLACK : GxEPD_WHITE);
    Watchy::display.setFont(font);
    int16_t x1, y1;
    uint16_t w, h;
    Watchy::display.getTextBounds(label, 0, 0, &x1, &y1, &w, &h);
    Watchy::display.fillScreen(bgColor);
    int space = (200 - (rleImage->h + h)) / 3;
    drawRLEBitmap(Watchy::display, (200 - rleImage->w) / 2,
                  space, rleImage, fgColor);
    Watchy::display.setCursor((200 - w) / 2, (200 - h - space));
    Watchy::display.print(label);
  }
};
