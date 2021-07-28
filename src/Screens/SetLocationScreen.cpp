#include "SetLocationScreen.h"

#include <IPAddress.h>
#include <Wifi.h>
#include <esp_http_client.h>

#include "Fonts/FreeSans12pt7b.h"

// these should move into Watchy
RTC_DATA_ATTR float SetLocationScreen::lat = DEFAULT_LOCATION_LATITUDE;
RTC_DATA_ATTR float SetLocationScreen::lon = DEFAULT_LOCATION_LONGDITUDE;
RTC_DATA_ATTR char SetLocationScreen::timezone[50];
RTC_DATA_ATTR IPAddress SetLocationScreen::ip;
RTC_DATA_ATTR enum {
  ready = 0,
  waitingForWifi,
  waitingForHttp,
  success,
  wifiFailed,
  httpRequestFailed,
  httpResponseErr,
  numLocationStates
} setLocationState;

const char *locationStateMsgs[numLocationStates] = {"\nready",
                                                    "\nwaiting for wifi",
                                                    "\nwaiting for http",
                                                    "\nsuccess",
                                                    "\nwifi failed",
                                                    "\nhttp request failed",
                                                    "\nhttp response error"};

esp_err_t _http_event_handle(esp_http_client_event_t *evt) {
  if (evt->event_id == HTTP_EVENT_ON_DATA &&
      !esp_http_client_is_chunked_response(evt->client)) {
    JSONVar responseObject = JSON.parse((const char *)evt->data);
    SetLocationScreen::lat = double(responseObject["lat"]);
    SetLocationScreen::lon = double(responseObject["lon"]);
    strncpy(SetLocationScreen::timezone, responseObject["timezone"], 50);
    SetLocationScreen::ip.fromString(responseObject["query"]);
    setLocationState = success;
    Watchy::display.printf("\nlat %8.4f\nlon %8.4f", SetLocationScreen::lat,
                           SetLocationScreen::lon);
    Watchy::display.printf("\ntz %s", SetLocationScreen::timezone);
    Watchy::display.printf("\nip %s", SetLocationScreen::ip.toString().c_str());
    DEBUG("success\nlat %f, lon %f, tz %s, ip %s\n", SetLocationScreen::lat,
          SetLocationScreen::lon, SetLocationScreen::timezone,
          SetLocationScreen::ip.toString().c_str());
  }
  return ESP_OK;
}

void displayMsg(const char *s, uint16_t bgColor) {
  Watchy::display.fillScreen(bgColor);
  Watchy::display.print(s);
  Watchy::display.printf("\npress back to exit");
  Watchy::display.display(true);
  Watchy::display.setCursor(0, 0);
}

void SetLocationScreen::show() {
  // http://ip-api.com/json?fields=57792
  // {"status":"success","lat":-27.4649,"lon":153.028,"timezone":"Australia/Brisbane","query":"202.144.174.72"}
  Watchy::display.setFont(&FreeSans12pt7b);
  if (setLocationState != ready) {  // not sure how we get here...
    displayMsg(locationStateMsgs[setLocationState], bgColor);
    return;
  }
  setLocationState = waitingForWifi;
  displayMsg(locationStateMsgs[setLocationState], bgColor);
  Watchy::display.fillScreen(bgColor);
  if (Watchy::connectWiFi()) {
    esp_http_client_config_t config = {};
    config.url = "http://ip-api.com/json?fields=57792";
    config.event_handler = _http_event_handle;
    esp_http_client_handle_t client = esp_http_client_init(&config);
    esp_err_t err = esp_http_client_perform(client);

    if (err == ESP_OK) {
      auto httpResponseCode = esp_http_client_get_status_code(client);
      if (httpResponseCode != 200) {
        setLocationState = httpResponseErr;
        Watchy::display.printf("\nhttp failed code %d", httpResponseCode);
      }
    } else {
      setLocationState = httpRequestFailed;
      Watchy::display.printf("\nhttp request failed\nesp err %s",
                             esp_err_to_name(err));
    }
    Watchy::display.printf("\npress back to exit");
    Watchy::display.display(true);
    esp_http_client_cleanup(client);
    // turn off radios
    WiFi.mode(WIFI_OFF);
    btStop();
  } else {
    setLocationState = wifiFailed;
    displayMsg(locationStateMsgs[setLocationState], bgColor);
  }
}

void SetLocationScreen::back() {
  setLocationState = ready;
  Screen::back();
}