#include "GFXTextUtil.h"

#include <Adafruit_GFX.h>
#include <ctype.h>

/**
 * @brief when printing, how much width on the screen does this glyph require
 * 
 * @param c char to compute width
 * @param f font in use
 * @return int16_t "width" of this character (actual width + inter char spacing)
 */
int16_t charWidth(const char c, const GFXfont *f) {
  if (c < f->first) { return 0; }
  if (c > f->last) { return 0; }
  const GFXglyph g = f->glyph[c - f->first];
  return g.xAdvance;

}

/**
 * @brief how far does this glyph extend, relative to the baseline?
 * 
 * @param c character to compute
 * @param f font in use
 * @return int16_t how far above or below the baseline does this char extend?
 */
int16_t charDescent(const char c, const GFXfont *f) {
  if (c < f->first) { return 0; }
  if (c > f->last) { return 0; }
  const GFXglyph g = f->glyph[c - f->first];
  return g.yOffset + g.height;
}

/**
 * @brief remove all whitespace from front of t. Return trimmed string.
 * Whitespace is any non-printing (no black pixels) chars in f.
 *
 * @param t
 * @param f
 * @return const char*
 */
const char *trim(const char *t) {
  if (t == nullptr) { return t; }
  while (*t && isspace(*t)) { t++; }
  return t;
}

/**
 * @brief draw word wrapped text within a bounding box. Takes GFX, bounding box,
 *        text string, and font
 *
 * @param g Adafruit_GFX to draw on
 * @param x upper left of bounding box (left)
 * @param y upper left of bounding box (top)
 * @param w width of bounding box
 * @param h height of bounding box
 * @param t text to draw. will draw as much as fits
 * @param f font to use
 */
void drawWordWrappedText(Adafruit_GFX &g, int16_t x, int16_t y, int16_t w,
                         int16_t h, const char *t, const GFXfont *f) {
  if (!t) { return; }
  g.setFont(f);
  g.setTextWrap(false);  // clip (GFX used to have a bug that would auto-wrap on exact width)
  int16_t yPos = 0;      ///< current y position relative to top left of bounding box
  for (;;) {             // for each line line
    const char *startLine = t, *startWord = t;
    yPos += f->yAdvance;
    // find line end
    for (int16_t xPos = 0; *t && xPos < w; xPos += charWidth(*t, f)) {  
      if (!isalnum(*t)) { startWord = t + 1; };
      t++;
    }
    if (startWord == startLine) {
      // current word doesn't fit on one line. Hard wrap it
      startWord = t;
    }
    // print up to the beginning of the current word
    // check to make sure it fits in the bounding box
    int16_t maxY = charDescent(*startLine, f);
    for (const char *p = startLine; p < startWord; p++) {
      maxY = max(maxY, charDescent(*p, f));
    }
    if (yPos + maxY > h) {
      // this line's glyphs don't fit in the bounding box. we're done
      break;
    }
    g.setCursor(x, y+yPos);
    g.printf("%.*s", startWord - startLine, startLine);
    t = trim(startWord);  // trim any leading whitespace
  }
}
