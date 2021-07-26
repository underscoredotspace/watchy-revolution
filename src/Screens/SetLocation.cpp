#include "SetLocation.h"

#include <IPAddress.h>
#include <Wifi.h>
#include <esp_http_client.h>

RTC_DATA_ATTR float SetLocation::lat;
RTC_DATA_ATTR float SetLocation::lon;
RTC_DATA_ATTR char SetLocation::timezone[50];
RTC_DATA_ATTR IPAddress SetLocation::ip;
RTC_DATA_ATTR enum {
  ready,
  waitingForWifi,
  waitingForHttp,
  success,
  wifiFailed,
  httpRequestFailed,
  httpResponseErr
} setLocationState;

esp_err_t _http_event_handle(esp_http_client_event_t *evt) {
  switch (evt->event_id) {
    case HTTP_EVENT_ON_DATA:
      if (!esp_http_client_is_chunked_response(evt->client)) {
        JSONVar responseObject = JSON.parse((const char *)evt->data);
        SetLocation::lat = double(responseObject["lat"]);
        SetLocation::lon = double(responseObject["lon"]);
        strncpy(SetLocation::timezone, responseObject["timezone"], 50);
        SetLocation::ip.fromString(responseObject["query"]);
        setLocationState = success;
        Watchy::display.printf("\nsuccess");
        Watchy::display.printf("\nlat %8.4f\nlon %8.4f", SetLocation::lat,
                               SetLocation::lon);
        Watchy::display.printf("\ntz %s", SetLocation::timezone);
        Watchy::display.printf("\nip %s", SetLocation::ip.toString().c_str());
        Watchy::display.printf("\npress back to exit");
        DEBUG("success\nlat %f, lon %f, tz %s, ip %s\n", SetLocation::lat,
              SetLocation::lon, SetLocation::timezone,
              SetLocation::ip.toString().c_str());
      }
      break;
    default:
      break;
  }
  return ESP_OK;
}

void displayMsg(const char *s, uint16_t bgColor) {
  Watchy::display.fillScreen(bgColor);
  Watchy::display.setCursor(0, 0);
  Watchy::display.print(s);
  Watchy::display.display(true);
}

void printSetLocationState(uint16_t bgColor) {
  switch (setLocationState) {
    case ready:
      displayMsg("\nready\npress back to exit", bgColor);
      break;
    case waitingForWifi:
      displayMsg("\nwaiting for wifi\npress back to exit", bgColor);
      break;
    case waitingForHttp:
      displayMsg("\nwaiting for http\npress back to exit", bgColor);
      break;
    case success:
      displayMsg("\nsuccess\npress back to exit", bgColor);
      break;
    case wifiFailed:
      displayMsg("\nwifi failed\npress back to exit", bgColor);
      break;
    case httpRequestFailed:
      displayMsg("\nhttp request failed\npress back to exit", bgColor);
      break;
    case httpResponseErr:
      displayMsg("\nhttp response error\npress back to exit", bgColor);
      break;
    default:
      displayMsg("\npress back to exit\npress back to exit", bgColor);
      break;
  }
}

void SetLocation::show() {
  // http://ip-api.com/json?fields=57792
  // {"status":"success","lat":-27.4649,"lon":153.028,"timezone":"Australia/Brisbane","query":"202.144.174.72"}
  if (setLocationState != ready) {  // not sure how we get here...
    printSetLocationState(bgColor);
    return;
  }
  setLocationState = waitingForWifi;
  displayMsg("\nWaiting for Wifi", bgColor);
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
        Watchy::display.fillScreen(bgColor);
        Watchy::display.setCursor(0, 0);
        Watchy::display.printf("\nhttp failed code %d\npress back to exit",
                               httpResponseCode);
        Watchy::display.display(true);
      }
    } else {
      setLocationState = httpRequestFailed;
      Watchy::display.fillScreen(bgColor);
      Watchy::display.setCursor(0, 0);
      Watchy::display.printf(
          "\nhttp request failed\nesp err %s\npress back to exit",
          esp_err_to_name(err));
      Watchy::display.display(true);
    }
    esp_http_client_cleanup(client);
    // turn off radios
    WiFi.mode(WIFI_OFF);
    btStop();
  } else {
    setLocationState = wifiFailed;
    displayMsg("\nwifi not connected\npress back to exit", bgColor);
  }
}

void SetLocation::back() {
  setLocationState = ready;
  Screen::back();
}