#include "Watchy.h" //include the Watchy library
#include "Screen.h"
#include "TimeScreen.h"

Watchy m; //instantiate your watchface

void setup()
{
  TimeScreen ts(&m);
  m.setScreen(&ts);
  static RTC_DATA_ATTR int bootCount = 0;
  ++bootCount;
  Serial.begin(921600);
  DEBUG("serial initialized @ 921600\n");
  DEBUG("wakeup_reason #%d: %d\n", bootCount, esp_sleep_get_wakeup_cause());
  m.init(); //call init in setup
}

void loop()
{
  // this should never run, Watchy deep sleeps after init();
}