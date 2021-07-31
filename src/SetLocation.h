#pragma once

#include <IPAddress.h>

namespace Watchy_SetLocation {

extern float lat, lon;
extern char timezone[50];
extern uint32_t ip;

typedef enum {
  ok,
  connectWifiFail,
  badHttpResponseCode,
  chunkedDataResponse,
  espHttpClientRequestFail
} SetLocationResult;

extern SetLocationResult setLocation();
}  // namespace Watchy_SetLocation