#include "SetLocation.h"

#include <Arduino_JSON.h>
#include <IPAddress.h>
#include <esp_http_client.h>

#include "Watchy.h"  // for connectWiFi

namespace Watchy_SetLocation {

constexpr const float DEFAULT_LOCATION_LATITUDE = 37.8136;
constexpr const float DEFAULT_LOCATION_LONGDITUDE = -144.9631;
RTC_DATA_ATTR float lat = DEFAULT_LOCATION_LATITUDE;
RTC_DATA_ATTR float lon = DEFAULT_LOCATION_LONGDITUDE;
RTC_DATA_ATTR char timezone[50];
RTC_DATA_ATTR IPAddress ip;
RTC_DATA_ATTR time_t lastSetLocationTS;  // can use this to throttle

esp_err_t _http_event_handle(esp_http_client_event_t *evt) {
  if (evt->event_id == HTTP_EVENT_ON_DATA) {
    if (esp_http_client_is_chunked_response(evt->client)) {
      return ESP_ERR_INVALID_RESPONSE;
    }
    JSONVar responseObject = JSON.parse((const char *)evt->data);
    lat = double(responseObject["lat"]);
    lon = double(responseObject["lon"]);
    strncpy(timezone, responseObject["timezone"], 50);
    ip.fromString(responseObject["query"]);
    lastSetLocationTS = now();
  }
  return ESP_OK;
}

bool setLocation() {
  // http://ip-api.com/json?fields=57792
  // {"status":"success","lat":-27.4649,"lon":153.028,"timezone":"Australia/Brisbane","query":"202.144.174.72"}
  if (!Watchy::connectWiFi()) {
    return false;
  }
  bool res = false;
  esp_http_client_config_t config = {};
  config.url = "http://ip-api.com/json?fields=57792";
  config.event_handler = _http_event_handle;
  esp_http_client_handle_t client = esp_http_client_init(&config);
  esp_err_t err = esp_http_client_perform(client);
  if (err == ESP_OK) {
    int httpResponseCode = esp_http_client_get_status_code(client);
    if (httpResponseCode != 200) {
      res = false;
    } else {
      res = true;
    }
  } else {
    res = false;
  }
  esp_http_client_cleanup(client);
  // turn off radios
  WiFi.mode(WIFI_OFF);
  btStop();
  return res;
}
}  // namespace Watchy_SetLocation