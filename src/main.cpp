#include "Watchy.h" //include the Watchy library
#include "Screen.h"
#include "TimeScreen.h"

TimeScreen ts;

void setup()
{
  Serial.begin(921600);
  DEBUG("  setup %d\n", esp_sleep_get_wakeup_cause());
  ts.setUp(&ts);
  ts.setDown(&ts);
  ts.setBack(&ts);
  ts.setMenu(Watchy::menuScreenP);
  Watchy::watchFace = &ts;
  Watchy::init(); //call init in setup
}

void loop()
{
  // this should never run, Watchy deep sleeps after init();
}