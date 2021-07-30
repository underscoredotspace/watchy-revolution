#include "ShowBatteryScreen.h"

#include "Fonts/FreeSans24pt7b.h"
#include "Watchy.h"

void ShowBatteryScreen::show() {
  Watchy::display.fillScreen(bgColor);
  Watchy::display.setFont(&FreeSans24pt7b);
  Watchy::display.printf("\n\nbattery\n%4.2fv\n", Watchy::getBatteryVoltage());
}
