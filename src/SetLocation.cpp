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
RTC_DATA_ATTR uint32_t ip;
RTC_DATA_ATTR time_t lastSetLocationTS;  // can use this to throttle

esp_err_t _http_event_handle(esp_http_client_event_t *evt) {
  if (evt->event_id == HTTP_EVENT_ON_DATA) {
    if (esp_http_client_is_chunked_response(evt->client)) {
      *(SetLocationResult *)evt->user_data = chunkedDataResponse;
      return ESP_OK;
    }
    JSONVar responseObject = JSON.parse((const char *)evt->data);
    lat = double(responseObject["lat"]);
    lon = double(responseObject["lon"]);
    strncpy(timezone, responseObject["timezone"], 50);
    IPAddress myIp;
    myIp.fromString((const char *)responseObject["query"]);
    ip = myIp;
    LOGI("%s %s %08lx", (const char *)responseObject["query"],
         myIp.toString().c_str(), Watchy_SetLocation::ip);
    lastSetLocationTS = now();
    *(SetLocationResult *)evt->user_data = ok;
  }
  return ESP_OK;
}

SetLocationResult setLocation() {
  // http://ip-api.com/json?fields=57792
  // {"status":"success","lat":-27.4649,"lon":153.028,"timezone":"Australia/Brisbane","query":"202.144.174.72"}
  if (!Watchy::connectWiFi()) {
    return connectWifiFail;
  }
  SetLocationResult res = ok;
  esp_http_client_config_t config = {};
  config.url = "http://ip-api.com/json?fields=57792";
  config.event_handler = _http_event_handle;
  config.user_data = &res;
  esp_http_client_handle_t client = esp_http_client_init(&config);
  esp_err_t err = esp_http_client_perform(client);
  if (err == ESP_OK) {
    int httpResponseCode = esp_http_client_get_status_code(client);
    if (httpResponseCode != 200) {
      res = badHttpResponseCode;
    } else {
      res = ok;
    }
  } else {
    res = espHttpClientRequestFail;
  }
  esp_http_client_cleanup(client);
  // turn off radios
  WiFi.mode(WIFI_OFF);
  btStop();
  return res;
}
}  // namespace Watchy_SetLocation