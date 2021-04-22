#include <DFSparks.h>

#if !defined(ESP8266) && !defined(ESP32)
#  error "Unexpected board"
#endif

namespace dfsparks {

struct VestPixels : public PixelMap {
  VestPixels() : PixelMap(MATRIX_WIDTH, MATRIX_HEIGHT, LEDNUM, pixelMap, true) {
  }

  void doSetColor(int i,RgbaColor color) final {
    leds[i] = CRGB(color.red, color.green, color.blue);       
  } 

  CRGB leds[MATRIX_WIDTH*MATRIX_HEIGHT]; 
  
} pixels;


Esp8266Network network("FISHLIGHT", "155155155");
//Esp8266Network network("detour", "thebeatenpath");

NetworkPlayer player(pixels, network);

void vestSetup(void) {
  logLevel = debugLevel;
  
  Serial.begin(115200);

  setupButtons();

  // Write FastLED configuration data
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(pixels.leds,
                                    sizeof(pixels.leds)/sizeof(*pixels.leds));


  pushBrightness();
}

void vestLoop(void) {
  uint32_t currentMillis = (uint32_t)millis();
  updateButtons(currentMillis); // read, debounce, and process the buttons
  doButtons(player, currentMillis); // perform actions based on button state
  network.poll();
  player.render();
  FastLED.show();
}

} // namespace dfsparks