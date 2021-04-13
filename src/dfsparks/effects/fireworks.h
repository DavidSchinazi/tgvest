#ifndef DFSPARKS_EFFECT_FIREWORKS_H
#define DFSPARKS_EFFECT_FIREWORKS_H
#include "dfsparks/effect.h"

namespace dfsparks {

class Fireworks : public Effect {
  void doRender(Pixels &pixels) final {
    int32_t elapsed = timeElapsed();

      byte sparksDone = 0;

      // startup tasks
      if (effectInit == false) {
        effectInit = true;
        effectDelay = 5;
        gSkyburst = 1;
        fadeActive = 0;
      }


      if (boom) {
        pixels.fill(RgbaColor(0, 0, 0));
        boom = false;
      } else {
        fadeAll(40);
      }

      if (sparkLife > 0) sparkLife--;


      for( byte b = 0; b < NUM_SPARKS; b++) {
        if (sparkLife <= 0) gSparks[b].show = 0;
        gSparks[b].Move();
        gSparks[b].Draw();
        sparksDone += gSparks[b].show;
      }

      if (sparksDone == 0) gSkyburst = 1;
      //Serial.println(sparksDone);

      if( gSkyburst) {
        effectDelay = 5;
        sparkLife = random(16,150);
        CRGB color;
        hsv2rgb_rainbow( CHSV( random8(), 255, 255), color);
        accum88 sx = random(127-64,127+64)<<8;
        accum88 sy = random(127-16,127+16)<<8;
        for( byte b = 0; b < NUM_SPARKS; b++) {
          gSparks[b].Skyburst(sx, sy, 0, color);
        }
        gSkyburst = 0;
        sparksDone = 0;
        fillAll(CRGB::Gray);
        boom = true;
      }
  }
  int sparkLife = 50;
  boolean boom = false;
  bool effectInit = false;
};

} // namespace dfsparks

#endif /* DFSPARKS_EFFECT_FIREWORKS_H */
