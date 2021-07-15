#include "Watchy.h"
#include "Screen.h"
#include "MenuScreen.h"
#include "AccelerometerScreen.h"

using namespace Watchy;

AccelerometerScreen accelerometerScreen;

void AccelerometerScreen::show()
{
    // TODO consider using the RTC to tell us to update every 200 ms
    display.setFont(&FreeMonoBold9pt7b);

    Accel acc;

    const int interval = 200;
    long timeout = millis() + interval;

    pinMode(BACK_BTN_PIN, INPUT);

    while (!Watchy::pollButtonsAndDispatch())
    {
        if (millis() < timeout)
        {
            continue;
        }

        timeout = millis() + interval;

        display.fillScreen(bgColor);
        display.setCursor(0, 30);

        // Get acceleration data
        if (!sensor.getAccel(acc))
        {
            display.println("getAccel FAIL");
            continue;
        }

        display.printf("  X: %d\n  Y: %d\n  Z: %d\n", acc.x, acc.y, acc.z);

        display.setCursor(30, 130);
        uint8_t direction = sensor.getDirection();
        switch (direction)
        {
        case DIRECTION_DISP_DOWN:
            display.println("FACE DOWN");
            break;
        case DIRECTION_DISP_UP:
            display.println("FACE UP");
            break;
        case DIRECTION_BOTTOM_EDGE:
            display.println("BOTTOM EDGE");
            break;
        case DIRECTION_TOP_EDGE:
            display.println("TOP EDGE");
            break;
        case DIRECTION_RIGHT_EDGE:
            display.println("RIGHT EDGE");
            break;
        case DIRECTION_LEFT_EDGE:
            display.println("LEFT EDGE");
            break;
        default:
            display.println("ERROR!!!");
            break;
        }
        display.display(true); //partial refresh
    }
    setScreen(&menuScreen);
}

void AccelerometerScreen::up() { setScreen(&menuScreen); }
void AccelerometerScreen::down() { setScreen(&menuScreen); }
void AccelerometerScreen::back() { setScreen(&menuScreen); }
void AccelerometerScreen::menu() { setScreen(&menuScreen); }