#pragma once

#include <IPAddress.h>

namespace Watchy_GetLocation {

typedef struct {
  float lat;
  float lon;
  char timezone[128]; // POSIX tz spec https://developer.ibm.com/articles/au-aix-posix/
} location;

extern location currentLocation;

extern const location* getLocation();
}  // namespace Watchy_GetLocation