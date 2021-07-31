#include "SetLocationScreen.h"

#include "Fonts/FreeSans12pt7b.h"
#include "SetLocation.h"
#include "IPAddress.h"

typedef enum {
  ready,
  wait,
  success,
  fail,
  numSetLocationStates
} SetLocationState;

RTC_DATA_ATTR SetLocationState setLocationState = ready;
const char *setLocationStateMsgs[numSetLocationStates] = {
    "\nready", "\nwaiting for sync", "\nsuccess", "\nfail"};

void printLocation() {
  Watchy::display.printf("\nsuccess\nlat %.4f\nlon %.4f",
                         Watchy_SetLocation::lat, Watchy_SetLocation::lon);
  Watchy::display.printf("\n%s\n", Watchy_SetLocation::timezone);
  Watchy::display.print(IPAddress(Watchy_SetLocation::ip));
  LOGI("%08lx", Watchy_SetLocation::ip);
}

void printSetLocationState() {
  if (setLocationState == success) {
    printLocation();
    return;
  }
  Watchy::display.print(setLocationStateMsgs[setLocationState]);
  Watchy::display.print("\npress back to exit");
}

void SetLocationScreen::show() {
  Watchy::display.fillScreen(bgColor);
  Watchy::display.setFont(&FreeSans12pt7b);
  if (setLocationState != ready) {
    printSetLocationState();
    return;
  }
  setLocationState = wait;
  printSetLocationState();
  Watchy::display.display(true);
  Watchy::display.setCursor(0, 0);
  Watchy::display.fillScreen(bgColor);

  if (Watchy_SetLocation::setLocation() == Watchy_SetLocation::ok) {
    setLocationState = success;
  } else {
    setLocationState = fail;
  }
  printSetLocationState();
}

void SetLocationScreen::back() {
  setLocationState = ready;
  Screen::back();
}