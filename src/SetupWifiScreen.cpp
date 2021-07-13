#include "Watchy.h"
#include "Screen.h"
#include "MenuScreen.h"
#include "SetupWifiScreen.h"

using namespace Watchy;

SetupWifiScreen setupWifiScreen;

const int WIFI_AP_TIMEOUT = 60;

void _configModeCallback (WiFiManager *myWiFiManager);

void SetupWifiScreen::show()
{
    WiFiManager wifiManager;
    wifiManager.resetSettings();
    wifiManager.setTimeout(WIFI_AP_TIMEOUT);
    wifiManager.setAPCallback(_configModeCallback);
    if (!wifiManager.autoConnect(WIFI_AP_SSID))
    {                           //WiFi setup failed
        display.init(0, false); //_initial_refresh to false to prevent full update on init
        display.setFullWindow();
        display.fillScreen(GxEPD_BLACK);
        display.setFont(&FreeMonoBold9pt7b);
        display.setTextColor(GxEPD_WHITE);
        display.setCursor(0, 30);
        display.println("Setup failed &");
        display.println("timed out!");
        display.display(false); //full refresh
        display.hibernate();
    }
    else
    {
        display.init(0, false); //_initial_refresh to false to prevent full update on init
        display.setFullWindow();
        display.fillScreen(GxEPD_BLACK);
        display.setFont(&FreeMonoBold9pt7b);
        display.setTextColor(GxEPD_WHITE);
        display.println("Connected to");
        display.println(WiFi.SSID());
        display.display(false); //full refresh
        display.hibernate();
    }
    //turn off radios
    WiFi.mode(WIFI_OFF);
    btStop();
}

void SetupWifiScreen::up() { setScreen(&menuScreen); }
void SetupWifiScreen::down() { setScreen(&menuScreen); }
void SetupWifiScreen::back() { setScreen(&menuScreen); }
void SetupWifiScreen::menu() { setScreen(&menuScreen); }

void _configModeCallback (WiFiManager *myWiFiManager) {
  display.init(0, false); //_initial_refresh to false to prevent full update on init
  display.setFullWindow();
  display.fillScreen(GxEPD_BLACK);
  display.setFont(&FreeMonoBold9pt7b);
  display.setTextColor(GxEPD_WHITE);
  display.setCursor(0, 30);
  display.println("Connect to");
  display.print("SSID: ");
  display.println(WIFI_AP_SSID);
  display.print("IP: ");
  display.println(WiFi.softAPIP());
  display.display(false); //full refresh
  display.hibernate();
}