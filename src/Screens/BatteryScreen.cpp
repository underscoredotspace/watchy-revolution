#include "BatteryScreen.h"

#include "Adafruit_GFX.h"
#include "Fonts/FreeSans24pt7b.h"
#include "Watchy.h"
#include "icons.h"

void BatteryScreen::show() {
  constexpr const char *label = "battery";
  const uint16_t fgColor = (bgColor == GxEPD_WHITE ? GxEPD_BLACK : GxEPD_WHITE);
  Watchy::display.setFont(&FreeSans24pt7b);
  int16_t x1, y1;
  uint16_t w, h;
  drawRLEBitmap(Watchy::display, (200 - 96) / 2, (200 - 96) / 2 / 2,
                &rle_battery, fgColor);
  Watchy::display.getTextBounds(label, 0, 0, &x1, &y1, &w, &h);
  Watchy::display.setCursor((200 - w) / 2, 170);
  Watchy::display.print(label);
}
