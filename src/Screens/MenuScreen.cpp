#include "MenuScreen.h"

#include "Watchy.h"

using namespace Watchy;

const int MENU_HEIGHT = 30;

RTC_DATA_ATTR int8_t MenuScreen::index;

MenuScreen::MenuScreen(MenuItem *mis, const int8_t ms) : Screen(), items(mis), size(ms) {
  for (int i = 0; i < size; i++) {
    if (items[i].screen != nullptr) {
      items[i].screen->parent = this;
    }
  }
}

void MenuScreen::show() {
  const uint16_t fgColor =
      (screen->bgColor == GxEPD_WHITE ? GxEPD_BLACK : GxEPD_WHITE);
  Watchy::display.fillScreen(bgColor);
  const GFXfont &font = FreeMonoBold9pt7b;
  display.setFont(&font);

  int16_t yPos = 0;
  for (uint8_t i = 0; i < size; i++) {
    if (i == index) {
      display.fillRect(0, yPos+font.yAdvance/2, DISPLAY_WIDTH, MENU_HEIGHT, fgColor);
      display.setTextColor(bgColor);
    } else {
      display.setTextColor(fgColor);
    }
    yPos += MENU_HEIGHT;
    display.setCursor(0, yPos);
    display.println(items[i].name);
  }
}

void MenuScreen::menu() { Watchy::setScreen(items[index].screen); }

void MenuScreen::back() { setScreen(parent); }

void MenuScreen::up() {
  index--;
  if (index < 0) {
    index = size - 1;
  }
  showWatchFace(true);
}

void MenuScreen::down() {
  index++;
  if (index >= size) {
    index = 0;
  }
  showWatchFace(true);
}
