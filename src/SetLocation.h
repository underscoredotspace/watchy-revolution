#pragma once

#include <IPAddress.h>

namespace Watchy_SetLocation {

  extern float lat, lon;
  extern char timezone[50];
  extern IPAddress ip;

  extern bool setLocation(); // returns false on failure
}  // namespace Watchy_SetLocation