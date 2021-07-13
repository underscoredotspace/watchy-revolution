#include "Watchy.h" //include the Watchy library
#include "Screen.h"
#include "TimeScreen.h"

using namespace Watchy;

void setup()
{
  Serial.begin(921600);
  DEBUG("  setup %d\n", esp_sleep_get_wakeup_cause());
  if (screen == nullptr) { screen = &timeScreen; } // on reset, set current screen

  Watchy::init(); //call init in setup
}

void loop()
{
  // this should never run, Watchy deep sleeps after init();
}