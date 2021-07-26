#include "ShowWifi.h"

#include "Fonts/FreeSans24pt7b.h"
#include "Watchy.h"

void ShowWifi::show() {
  Watchy::display.setFont(&FreeSans24pt7b);
  Watchy::display.setCursor(0, 0);
  Watchy::display.printf("\nwifi\nis");
  if (!Watchy::WIFI_CONFIGURED) {
    Watchy::display.printf(" not");
  }
  Watchy::display.printf("\nset up\n");
}
