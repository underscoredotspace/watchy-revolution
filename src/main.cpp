#include "Watchy.h" //include the Watchy library
#include "Screen.h"
#include "TimeScreen.h"

Watchy m; //instantiate your watchface
TimeScreen ts(&m);

void setup()
{
  Serial.begin(921600);
  DEBUG("serial initialized @ 921600\n");
  if (m.screen == nullptr)
  {
    m.setScreen(&ts);
  }
  m.init(); //call init in setup
}

void loop()
{
  // this should never run, Watchy deep sleeps after init();
}