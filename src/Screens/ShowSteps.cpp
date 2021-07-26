#include "ShowStepsScreen.h"

#include "Fonts/FreeSans24pt7b.h"
#include "SettingsScreen.h"
#include "StepsScreen.h"
#include "Watchy.h"

void ShowStepsScreen::show() {
  Watchy::display.setFont(&FreeSans24pt7b);
  Watchy::display.setCursor(0, 0);
  Watchy::display.printf("\n%d\nsteps", Watchy::sensor.getCounter());
}
