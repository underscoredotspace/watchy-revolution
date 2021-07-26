#include "config.h"  // should be first

#include "SyncTimeScreen.h"

#include <Wifi.h>

#include "Adafruit_GFX.h"
#include "Fonts/FreeSans12pt7b.h"
#include "Watchy.h"
#include "icons.h"
#include "time.h"

RTC_DATA_ATTR const char *SyncTimeScreen::tz = TZ;
RTC_DATA_ATTR const char *SyncTimeScreen::ntpServer = NTP_SERVER;
RTC_DATA_ATTR enum SyncState {
  ready,
  waitingForSync,
  wifiFailed,
  syncFailed,
  syncSucceeded
} syncState = ready;

bool showSyncState() {
  switch (syncState) {
    case ready:
      return false;
    case waitingForSync:
      Watchy::display.print("\nsynchronizing");
      break;
    case wifiFailed:
      Watchy::display.print("\nwifi connect failed");
      break;
    case syncFailed:
      Watchy::display.print("\nsync failed");
      break;
    case syncSucceeded:
      Watchy::display.print("\nsuccess");
      break;
    default:
      break;
  }
  Watchy::display.print("\npress back to exit");
  return true;
}

void timeSyncCallback(struct timeval *tv) {
  // consider using tv.tv_usec as well
  struct tm t;
  localtime_r(&tv->tv_sec, &t);
  Watchy::display.print(&t, "\n%A\n%B %d %Y\n%H.%M.%S");
  tmElements_t te;
  breakTime(tv->tv_sec, te);
  Watchy::RTC.write(te);
  syncState = syncSucceeded;
  showSyncState();
}

void SyncTimeScreen::show() {
  Watchy::display.setFont(&FreeSans12pt7b);
  Watchy::display.setCursor(0, 0);
  if (showSyncState()) {
    return;
  }
  time_t tt;
  time(&tt);
  syncState = waitingForSync;
  showSyncState();
  Watchy::display.display(true);
  Watchy::display.fillScreen(bgColor);
  Watchy::display.setCursor(0, 0);

  if (!Watchy::connectWiFi()) {
    syncState = wifiFailed;
    showSyncState();
    return;
  }
  sntp_set_time_sync_notification_cb(timeSyncCallback);
  configTzTime(tz, ntpServer);
  uint32_t timeout = millis() + 5000;
  while (millis() < timeout) {
    if (syncState == syncSucceeded) {
      return;
    }
    delay(10);
  }
  syncState = syncFailed;
  showSyncState();
}

void SyncTimeScreen::back() {
  syncState = ready;
  Screen::back();
}

// RTC does not know about TZ
// so DST has to be in app code
// so RTC must store UTC
// RTC computes leap year so you want year to be correct
// according to how the RTC represents it