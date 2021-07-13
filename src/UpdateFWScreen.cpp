#include "Watchy.h"
#include "Screen.h"
#include "MenuScreen.h"
#include "UpdateFWScreen.h"

using namespace Watchy;

UpdateFWScreen updateFWScreen;

void updateFWBegin();

void UpdateFWScreen::show()
{
    display.init(0, false); //_initial_refresh to false to prevent full update on init
    display.setFont(&FreeMonoBold9pt7b);
    display.setCursor(0, 30);
    display.println("Please Visit");
    display.println("watchy.sqfmi.com");
    display.println("with a Bluetooth");
    display.println("enabled device");
    display.println(" ");
    display.println("Press menu button");
    display.println("again when ready");
    display.println(" ");
    display.println("Keep USB powered");
    display.display(false); //full refresh
    display.hibernate();
}
void UpdateFWScreen::up() { setScreen(&menuScreen); }
void UpdateFWScreen::down() { setScreen(&menuScreen); }
void UpdateFWScreen::back() { setScreen(&menuScreen); }
void UpdateFWScreen::menu() { updateFWBegin(); }

void updateFWBegin(){
    display.setFont(&FreeMonoBold9pt7b);
    display.setCursor(0, 30);
    display.println("Bluetooth Started");
    display.println(" ");
    display.println("Watchy BLE OTA");
    display.println(" ");
    display.println("Waiting for");
    display.println("connection...");
    display.display(false); //full refresh

    BLE BT;
    BT.begin("Watchy BLE OTA");
    int prevStatus = -1;
    int currentStatus;

    while(1){
    currentStatus = BT.updateStatus();
    if(prevStatus != currentStatus || prevStatus == 1){
        if(currentStatus == 0){
        display.setFont(&FreeMonoBold9pt7b);
        display.setCursor(0, 30);
        display.println("BLE Connected!");
        display.println(" ");
        display.println("Waiting for");
        display.println("upload...");
        display.display(false); //full refresh
        }
        if(currentStatus == 1){
        display.setFont(&FreeMonoBold9pt7b);
        display.setCursor(0, 30);
        display.println("Downloading");
        display.println("firmware:");
        display.println(" ");
        display.print(BT.howManyBytes());
        display.println(" bytes");
        display.display(true); //partial refresh        
        }
        if(currentStatus == 2){
        display.setFont(&FreeMonoBold9pt7b);
        display.setCursor(0, 30);
        display.println("Download");
        display.println("completed!");
        display.println(" ");
        display.println("Rebooting...");
        display.display(false); //full refresh

        delay(2000);
        esp_restart();           
        }
        if(currentStatus == 4){
        display.setFont(&FreeMonoBold9pt7b);
        display.setCursor(0, 30);
        display.println("BLE Disconnected!");
        display.println(" ");
        display.println("exiting...");
        display.display(false); //full refresh
        delay(1000);
        break;
        }
        prevStatus = currentStatus;
    }
    delay(100);
    }

    //turn off radios
    WiFi.mode(WIFI_OFF);
    btStop();
    setScreen(&menuScreen);
}