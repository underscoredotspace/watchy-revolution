#include "ShowBattery.h"

#include "Fonts/FreeSans24pt7b.h"
#include "Watchy.h"

void ShowBattery::show() {
  Watchy::display.setFont(&FreeSans24pt7b);
  Watchy::display.setCursor(0, 0);
  Watchy::display.printf("\n\nbattery\n%4.2fv\n", Watchy::getBatteryVoltage());
}
