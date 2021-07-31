#pragma once

namespace Watchy_SyncTime {
// POSIX tz spec https://developer.ibm.com/articles/au-aix-posix/
extern const char *tz;
extern const char *ntpServer;

typedef enum {
  ready,
  waiting,
  wifiFailed,
  timeout,
  success,
  numSyncStates
} SyncResult;

extern SyncResult syncTime();
};  // namespace Watchy_SyncTime