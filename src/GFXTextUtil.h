#pragma once

#include <Adafruit_GFX.h>

/**
 * @brief draws a text string inside a rectangular area of an Adafruit GFX. Wraps the text on
 *        word boundaries (any non-alphanumeric.) Fits as much text in the area as possible.
 *        uses the current font and font color.
 * 
 * @param g an Adafruit GFX object
 * @param x upper left corner of the viewport
 * @param y upper left corner of the viewport
 * @param w width of the viewport
 * @param h height of the viewport
 * @param t text to draw
 * @param f the font to draw the text
 */
void drawWordWrappedText(Adafruit_GFX &g, int16_t x, int16_t y, int16_t w,
                         int16_t h, const char *t, const GFXfont *f);
