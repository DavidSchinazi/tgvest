#include <WiFiUdp.h>
#include <Adafruit_NeoPixel.h>
#include <DFSparks.h>
using namespace dfsparks;

const int STATUS_PIN = 2; // builtin led
const int RING_PIN = 5; // D1
const int BUTTON_PIN = 13; // D7
const int NUM_LEDS = 12;

Adafruit_NeoPixel strip(NUM_LEDS, RING_PIN, NEO_GRB + NEO_KHZ800);

struct RingPixels : public VerticalStrand {
  RingPixels() : VerticalStrand(NUM_LEDS) {}

  void doSetColor(int i, RgbaColor color) final {
    strip.setPixelColor(i, color.red, color.green, color.blue);       
  };  
} pixels;

Esp8266Network network("FISHLIGHT", "155155155");
//Esp8266Network network("detour", "thebeatenpath");
NetworkPlayer player(pixels, network);

void setup()
{
  logLevel = debugLevel;

  Serial.begin(115200);
  Serial.println();
  Serial.println();

  // Initialize LED strip
  strip.begin();
  strip.show(); 
  strip.setPixelColor(0, 0xff0000);
  strip.show();

  pinMode(STATUS_PIN, OUTPUT);
  pinMode(RING_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT);
}

void loop()
{
  static int prevButtonState = LOW;
  static int pressTs = 0; 
  int buttonState = digitalRead(BUTTON_PIN);
  if (buttonState == HIGH && prevButtonState == LOW) {
    pressTs = millis();
  }
  else if (buttonState == LOW && prevButtonState == HIGH) {
      if (millis() - pressTs < 500) { // short press
         player.next();     
      }
      else {
        player.showStatus();
      }
  } 
  prevButtonState = buttonState;
  network.poll();
  player.render();
  strip.show();
  delay(10);
}