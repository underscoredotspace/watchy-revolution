#include "SyncTime.h"

#include <Wifi.h>

#include "Watchy.h"
#include "config.h"  // should be first
#include "time.h"

namespace Watchy_SyncTime {

RTC_DATA_ATTR const char *tz = TZ;
RTC_DATA_ATTR const char *ntpServer = NTP_SERVER;

// RTC does not know about TZ
// so DST has to be in app code
// so RTC must store UTC
// RTC computes leap year so you want year to be correct
// according to how the RTC represents it

void timeSyncCallback(struct timeval *tv) {
  // consider using tv.tv_usec as well
  setTime(tv->tv_sec);          // set system time
  Watchy::RTC.set(tv->tv_sec);  // set RTC
  sntp_set_sync_status(SNTP_SYNC_STATUS_COMPLETED);
}

void syncTime() {
  if (!Watchy::connectWiFi()) {
    return;  // failed
  }
  if (sntp_get_sync_status() != SNTP_SYNC_STATUS_RESET) {
    // SNTP busy
    LOGI("%d", sntp_get_sync_status());
    return;
  }
  sntp_set_time_sync_notification_cb(timeSyncCallback);
  configTzTime(tz, ntpServer);
  uint32_t timeout = millis() + 5000;  // 5 sec timeout
  while (millis() < timeout &&
         sntp_get_sync_status() != SNTP_SYNC_STATUS_COMPLETED) {
    delay(10);  // or yield
  }
}
}  // namespace Watchy_SyncTime