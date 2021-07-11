#include "Watchy.h" //include the Watchy library
#include "Screen.h"
#include "TimeScreen.h"

TimeScreen ts;

void setup()
{
  Serial.begin(921600);
  DEBUG("\nsetup %d\n", esp_sleep_get_wakeup_cause());
  Watchy::watchFace = &ts;
  Watchy::init(); //call init in setup
}

void loop()
{
  // this should never run, Watchy deep sleeps after init();
}