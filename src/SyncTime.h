#pragma once

namespace Watchy_SyncTime {
// POSIX tz spec https://developer.ibm.com/articles/au-aix-posix/
extern const char *tz;
extern const char *ntpServer;

extern void syncTime();
};  // namespace Watchy_SyncTime