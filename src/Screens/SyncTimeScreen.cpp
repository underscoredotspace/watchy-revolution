#include "SyncTimeScreen.h"

#include "config.h"  // should be first

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
  syncSucceeded,
  numSyncStates
} syncState = ready;

const char *syncStateMsgs[numSyncStates] = {"\nready", "\nsynchronizing",
                                            "\nwifi connect failed",
                                            "\nsync failed", "\nsuccess"};

void timeSyncCallback(struct timeval *tv) {
  // consider using tv.tv_usec as well
  struct tm t;
  localtime_r(&tv->tv_sec, &t);
  Watchy::display.print(&t, "\n%A\n%B %d %Y\n%H.%M.%S");
  tmElements_t te;
  breakTime(tv->tv_sec, te);
  Watchy::RTC.write(te);
  syncState = syncSucceeded;
  Watchy::display.print(syncStateMsgs[syncState]);
}

void SyncTimeScreen::show() {
  Watchy::display.setFont(&FreeSans12pt7b);
  if (syncState != ready) {
    Watchy::display.print(syncStateMsgs[syncState]);
  } else {
    time_t tt;
    time(&tt);
    syncState = waitingForSync;
    Watchy::display.print(syncStateMsgs[syncState]);
    Watchy::display.display(true);
    Watchy::display.fillScreen(bgColor);
    Watchy::display.setCursor(0, 0);

    if (!Watchy::connectWiFi()) {
      syncState = wifiFailed;
      Watchy::display.print(syncStateMsgs[syncState]);
    } else {
      sntp_set_time_sync_notification_cb(timeSyncCallback);
      configTzTime(tz, ntpServer);
      uint32_t timeout = millis() + 5000;
      while (millis() < timeout && syncState == waitingForSync) {
        delay(10);
      }
      if (syncState == waitingForSync) {
        syncState = syncFailed;
        Watchy::display.print(syncStateMsgs[syncState]);
      }
    }
  }
  Watchy::display.print("\npress back to exit");
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