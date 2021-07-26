#include "Carousel.h"

#include "Watchy.h"

using namespace Watchy;

RTC_DATA_ATTR int8_t Carousel::index;

Carousel::Carousel(CarouselItem *cis, const int8_t cs, Screen *p)
    : Screen(p), items(cis), size(cs) {
  for (int i = 0; i < size; i++) {
    if (items[i].splash != nullptr) {
      items[i].splash->parent = this;
    }
    if (items[i].child != nullptr) {
      items[i].child->parent = this;
    }
  }
}

void Carousel::show() {
  Watchy::showWatchFace(true, items[index].splash);
}

void Carousel::menu() {
  if (items[index].child != nullptr) {
    Watchy::setScreen(items[index].child);
  }
}

void Carousel::back() {
  index = 0;
  show();
}

void Carousel::up() {
  index--;
  if (index < 0) {
    index = size - 1;
  }
  show();
}

void Carousel::down() {
  index++;
  if (index >= size) {
    index = 0;
  }
  show();
}
