#include "BuzzScreen.h"

#include "MenuScreen.h"
#include "Screen.h"
#include "Watchy.h"

using namespace Watchy;

BuzzScreen buzzScreen;

void vibMotor(uint8_t intervalMs = 100, uint8_t length = 20);

void BuzzScreen::show() {
  display.setFont(&FreeMonoBold9pt7b);
  display.setCursor(70, 80);
  display.println("Buzz!");
  display.display(true);  // TODO: delete me
  vibMotor();
  setScreen(&menuScreen);
}

void BuzzScreen::up() { setScreen(&menuScreen); }
void BuzzScreen::down() { setScreen(&menuScreen); }
void BuzzScreen::back() { setScreen(&menuScreen); }
void BuzzScreen::menu() { setScreen(&menuScreen); }

void vibMotor(uint8_t intervalMs, uint8_t length) {
  pinMode(VIB_MOTOR_PIN, OUTPUT);
  bool motorOn = false;
  for (int i = 0; i < length; i++) {
    motorOn = !motorOn;
    digitalWrite(VIB_MOTOR_PIN, motorOn);
    delay(intervalMs);
  }
}
