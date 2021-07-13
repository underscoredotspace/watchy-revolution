#include "Watchy.h" //include the Watchy library
#include "Screen.h"
#include "TimeScreen.h"

using namespace Watchy;

void setup()
{
  Serial.begin(921600);
  unsigned long m = micros();
  DEBUG("  %ld.%03ld: setup %d\n", m/1000, m%1000, esp_sleep_get_wakeup_cause());
  if (screen == nullptr) { screen = &timeScreen; } // on reset, set current screen

  Watchy::init(); //call init in setup
}

void loop()
{
  // this should never run, Watchy deep sleeps after init();
}