#include "ShowWifiScreen.h"

#include "OptimaLTStd22pt7b.h"
#include "Watchy.h"

void ShowWifiScreen::show() {
  Watchy::display.fillScreen(bgColor);
  Watchy::display.setFont(OptimaLTStd22pt7b);
  Watchy::display.printf("\nwifi\nis");
  if (!Watchy::WIFI_CONFIGURED) {
    Watchy::display.printf(" not");
  }
  Watchy::display.printf("\nset up\n");
}
