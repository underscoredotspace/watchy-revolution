#pragma once

#include "Adafruit_GFX.h"

typedef struct {
  const uint8_t w, h;
  const uint8_t *bitmap;
} rle;

void drawRLEBitmap(Adafruit_GFX &gfx, int16_t x, int16_t y, const rle *r,
                   uint16_t color);
