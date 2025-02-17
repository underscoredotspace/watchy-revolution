#include "ShowOrientationScreen.h"

#include "OptimaLTStd12pt7b.h"
#include "Watchy.h"

void ShowOrientationScreen::showMe() {
  Watchy::display.fillScreen(bgColor);
  Watchy::display.setCursor(0,0);
  Accel acc;
  if (Watchy::sensor.getAccel(acc)) {
    Watchy::display.printf("\n  X: %d", acc.x);
    Watchy::display.printf("\n  Y: %d", acc.y);
    Watchy::display.printf("\n  Z: %d", acc.z);
  } else {
    Watchy::display.print("\ncan't get accel");
  }
  Watchy::display.printf("\ndirection\n");
  uint8_t direction = Watchy::sensor.getDirection();
  switch (direction) {
    case DIRECTION_DISP_DOWN:
      Watchy::display.print("down");
      break;
    case DIRECTION_DISP_UP:
      Watchy::display.print("up");
      break;
    case DIRECTION_BOTTOM_EDGE:
      Watchy::display.print("bottom");
      break;
    case DIRECTION_TOP_EDGE:
      Watchy::display.print("top");
      break;
    case DIRECTION_RIGHT_EDGE:
      Watchy::display.print("right");
      break;
    case DIRECTION_LEFT_EDGE:
      Watchy::display.print("left");
      break;
    default:
      Watchy::display.printf("%d ???", direction);
      break;
  }
  Watchy::display.printf("\npress back to exit");
  Watchy::display.display(true);
}

void ShowOrientationScreen::show() {
  Watchy::display.fillScreen(bgColor);
  Watchy::display.setFont(OptimaLTStd12pt7b);
  showing = true;
  while (showing) {
    showMe();
    auto timeout = millis() + 200;
    while (millis() < timeout) {
      Watchy::pollButtonsAndDispatch();
      yield();
    }
  }
}

void ShowOrientationScreen::back() {
  showing = false;
  Screen::back();
}