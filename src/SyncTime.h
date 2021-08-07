#pragma once

namespace Watchy_SyncTime {
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