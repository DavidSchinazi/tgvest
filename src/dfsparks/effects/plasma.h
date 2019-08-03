#ifndef DFSPARKS_EFFECT_PLASMA_H
#define DFSPARKS_EFFECT_PLASMA_H
#include "dfsparks/color.h"
#include "dfsparks/effect.h"
#include "dfsparks/math.h"

#include <FastLED.h>

namespace dfsparks {

class Plasma : public Effect {
  void doRender(Pixels &pixels) final {
    int32_t elapsed = timeElapsed();
    int width = pixels.width();
    int height = pixels.height();

    uint8_t offset = speed * elapsed / 255;
    int plasVector = offset * 16;

    // Calculate current center of plasma pattern (can be offscreen)
    int xOffset = cos8(plasVector / 256);
    int yOffset = sin8(plasVector / 256);

    for (int i = 0; i < pixels.count(); ++i) {
      int x, y;
      pixels.coords(i, &x, &y);

      double xx = 16.0 * x / width;
      double yy = 16.0 * y / height;
      uint8_t hue = sin8(sqrt(square((xx - 7.5) * 10 + xOffset - 127) +
                              square((yy - 2) * 10 + yOffset - 127)) +
                         offset);

      pixels.setColor(i, hsl(hue, 255, 255));
    }
  }

  int32_t speed = 30;
};

static inline RgbaColor flToDf(CRGB ori) {
  return RgbaColor(ori.r, ori.g, ori.b);
}

enum OurColorPalette {
  OCPCloud = 0,
  OCPLava = 1,
  OCPOcean = 2,
  OCPForest = 3,
  OCPRainbow = 4,
  OCPParty = 5,
  OCPHeat = 6,
};

static inline RgbaColor colorFromOurPalette(OurColorPalette ocp, uint8_t color) {
#define RET_COLOR(c) return flToDf(ColorFromPalette(c##Colors_p, color, 255))
#define CASE_RET_COLOR(c) case OCP##c: RET_COLOR(c)
  switch (ocp) {
    CASE_RET_COLOR(Cloud);
    CASE_RET_COLOR(Lava);
    CASE_RET_COLOR(Ocean);
    CASE_RET_COLOR(Forest);
    CASE_RET_COLOR(Rainbow);
    CASE_RET_COLOR(Party);
    CASE_RET_COLOR(Heat);
  }
  RET_COLOR(Rainbow);
#undef CASE_RET_COLOR
#undef RET_COLOR
}

class SpinPlasma : public Effect {
  void doRender(Pixels &pixels) final {
    int32_t elapsed = timeElapsed();

    static const int32_t speed = 30;
    uint8_t offset = speed * elapsed / 255;
    int plasVector = offset;

    // Calculate current center of plasma pattern (can be offscreen)
    int xOffset = (cos8(plasVector)-127)/2;
    int yOffset = (sin8(plasVector)-127)/2;

    // Draw one frame of the animation into the LED array
    for (int i = 0; i < pixels.count(); ++i) {
      int x, y;
      pixels.coords(i, &x, &y);
      uint8_t color = sin8(sqrt(square(((float)x - 7.5) * 12 + xOffset) +
                                square(((float)y - 2) * 12 + yOffset)) + offset);
      pixels.setColor(i, colorFromOurPalette(ocp_, color));
    }
  }
  OurColorPalette ocp_ = OCPRainbow;
public:
  SpinPlasma(OurColorPalette ocp) : ocp_(ocp) {}
};

} // namespace dfsparks

#endif /* DFSPARKS_EFFECT_PLASMA_H */
