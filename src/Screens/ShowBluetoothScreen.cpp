#include "ShowBluetoothScreen.h"

#include "Fonts/FreeSans24pt7b.h"
#include "Watchy.h"

void ShowBluetoothScreen::show() {
  Watchy::display.fillScreen(bgColor);
  Watchy::display.setFont(&FreeSans24pt7b);
  Watchy::display.printf("\nbluetooth\nis");
  if (!Watchy::BLE_CONFIGURED) {
    Watchy::display.printf(" not");
  }
  Watchy::display.printf("\nset up\n");
}
