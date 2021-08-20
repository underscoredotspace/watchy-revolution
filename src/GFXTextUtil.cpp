#include "GFXTextUtil.h"

#include <Adafruit_GFX.h>
#include <ctype.h>

#include "config.h" // DEBUG

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
  g.setTextWrap(false);  // clip (GFX used to have a bug that would auto-wrap on
                         // exact width)
  const char *startLine = t;
  const char *startNextLine = t;
  int16_t startNextLineXPos = 0;
  // newline (from 0,0)
  int16_t xPos = 0;
  int16_t yPos = f->yAdvance;      ///< y position relative to top left of bounding box
  if (yPos > h) { return; }
  g.setCursor(x, y + yPos);
  for (char c = *t; ; c = *++t) {
    if (c != '\0' && c != '\n'){
      xPos += charWidth(c, f);
      LOGD("%3d %3d %3d %2d %c", w, startNextLineXPos, xPos, charWidth(c, f), c);
      if (xPos <= w) {
        if (yPos + charDescent(c, f) > h) { return; }  // text won't fit in bounding box. Done.
        if (!isalnum(c)) {
          startNextLine = t + 1;
          startNextLineXPos = xPos;
        }
        continue;
      }
    }
    if (startNextLine == startLine) {
      // current word will not fit on one line. hard break.
      startNextLine = t;
      startNextLineXPos = xPos;
    }
    LOGD("%.*s", startNextLine - startLine, startLine);
    for (; startLine < startNextLine; startLine++) { g.print(*startLine); }
    if (startLine == t) {
      // we aren't breaking a word, so consume any trailing whitespace
      while (*t && isspace(*t) && *t != '\n') { t++; }
      c = *t;
    }
    if (c == '\0') { return; } // end of text
    xPos -= startNextLineXPos;
    startNextLineXPos = 0;
    // newline
    if (*startLine == '\n') { startLine++; } // consume a LF if it's the next char
    yPos += f->yAdvance;
    if (yPos > h) { break; }
    g.setCursor(x, y + yPos);
  }
}
