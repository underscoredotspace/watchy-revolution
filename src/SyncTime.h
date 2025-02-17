#pragma once

#include <time.h>

namespace Watchy_SyncTime {

// ntpServer defaults to NTP_SERVER from config.h
// but can be set to a more specific ntp server if known
extern const char* ntpServer;

constexpr const char *DEFAULT_TIMEZONE = "UTC0"; // posix format

extern time_t lastSyncTimeTS; // timestamp of last successful syncTime
// sets RTC, now() and time() to UTC
// sets current timezone so that localtime works
extern void syncTime(const char* timezone = DEFAULT_TIMEZONE);
};  // namespace Watchy_SyncTime