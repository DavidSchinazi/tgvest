//   RGB Vest Demo Code
//
//   Use Version 3.0 or later https://github.com/FastLED/FastLED
//   ZIP file https://github.com/FastLED/FastLED/archive/master.zip
//

// fixes flickering: https://github.com/FastLED/FastLED/issues/306
#define FASTLED_ALLOW_INTERRUPTS 0

// you need this line or the feather huzzah or LoLin nodecmu pins will not be mapped properly
#define FASTLED_ESP8266_RAW_PIN_ORDER
#define ESP8266
#include <FastLED.h>
#include <DFSparks.h>

using namespace dfsparks;


// Data output to LEDs is on pin 5
#define LED_PIN  5

// Vest color order (Green/Red/Blue)
#define COLOR_ORDER GRB
#define CHIPSET     WS2812B

// D0 = GPIO 16
// D1 = GPIO  5
// D2 = GPIO  4
// D3 = GPIO  0 = bottom-right button
// D4 = GPIO  2
// 3.3V
// GND
// D5 = GPIO 14 = top-right button
// D6 = GPIO 12 = bottom-left button
// D7 = GPIO 13 = top-left button
// D8 = GPIO 15
// D9 = GPIO  3
//D10 = GPIO 1
// GND
// 3.3V

// Button settings
#define NUMBUTTONS 4
#define MODEBUTTON 13
#define BRIGHTNESSBUTTON 14
#define WIFIBUTTON 12
#define SPECIALBUTTON 0
uint8_t buttonPins[NUMBUTTONS] = {MODEBUTTON, BRIGHTNESSBUTTON, WIFIBUTTON, SPECIALBUTTON};

#define BTN_IDLE 0
#define BTN_DEBOUNCING 1
#define BTN_PRESSED 2
#define BTN_RELEASED 3
#define BTN_LONGPRESS 4
#define BTN_LONGPRESSREAD 5

#define BTN_DEBOUNCETIME 20
#define BTN_LONGPRESSTIME 1000

// Global maximum brightness value, maximum 255
//#define MAXBRIGHTNESS 120 
//#define STARTBRIGHTNESS 40
//#define BRIGHTNESS_INCREASE 51

// this oredring is silly but we need the highest to be last
// in case it blows the fuse in the USB battery pack
const uint8_t brightnessList[] = { 32, 64, 96, 8, 16, 128};
uint8_t brightnessCursor = 0;

#if 1

// David's Orange Vest

#define MATRIX_WIDTH 19
#define MATRIX_HEIGHT 20
#define LEDNUM 360 // == LAST_VISIBLE_LED, total number of physical leds

const int pixelMap[] = {
    360, 17, 16, 15, 14, 13, 12, 11, 10,  9,  8,  7,  6,  5,  4,  3,  2,  1,  0,
     18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35,360,
    360, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36,
     54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71,360,
    360, 89, 88, 87, 86, 85, 84, 83, 82, 81, 80, 79, 78, 77, 76, 75, 74, 73, 72,
     90, 91, 92, 93, 94, 95, 96, 97, 98, 99,100,101,102,103,104,105,106,107,360,
    360,125,124,123,122,121,120,119,118,117,116,115,114,113,112,111,110,109,108,
    126,127,128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,143,360,
    360,161,160,159,158,157,156,155,154,153,152,151,150,149,148,147,146,145,144,
    162,163,164,165,166,167,168,169,170,171,172,173,174,175,176,177,178,179,360,
    360,197,196,195,194,193,192,191,190,189,188,187,186,185,184,183,182,181,180,
    198,199,200,201,202,203,204,205,206,207,208,209,210,211,212,213,214,215,360,
    360,233,232,231,230,229,228,227,226,225,224,223,222,221,220,219,218,217,216,
    234,235,236,237,238,239,240,241,242,243,244,245,246,247,248,249,250,251,360,
    360,269,268,267,266,265,264,263,262,261,260,259,258,257,256,255,254,253,252,
    270,271,272,273,274,275,276,277,278,279,280,281,282,283,284,285,286,287,360,
    360,305,304,303,302,301,300,299,298,297,296,295,294,293,292,291,290,289,288,
    306,307,308,309,310,311,312,313,314,315,316,317,318,319,320,321,322,323,360,
    360,341,340,339,338,337,336,335,334,333,332,331,330,329,328,327,326,325,324,
    342,343,344,345,346,347,348,349,350,351,352,353,354,355,356,357,358,359,360
};

#elif 0

// 15-LED mode.
// Vest dimensions
#define MATRIX_WIDTH 15
#define MATRIX_HEIGHT 16
#define LEDNUM 215 // == LAST_VISIBLE_LED, total number of physical leds

const int pixelMap[] = {
   216,216,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9,216,216,216,
    10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24,
    25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39,
   216,216, 40, 41, 42, 43, 44, 45, 46, 47,216,216,216,216,216,
    48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 
    63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 
    78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 
    93, 94, 95, 96, 97, 98, 99,100,101,102,103,104,105,106,107,
   108,109,110,111,112,113,114,115,116,117,118,119,120,121,122,
   123,124,125,126,127,128,129,130,131,132,133,134,135,136,137,
   138,139,140,141,142,143,144,145,146,147,148,149,150,151,152,
   153,154,155,156,157,158,159,160,161,162,163,164,165,166,167,
   216,216,168,169,170,171,172,173,174,175,216,216,216,216,216,
   176,177,178,179,180,181,182,183,184,185,186,187,188,189,190,
   191,192,193,194,195,196,197,198,199,200,201,202,203,204,205,
   216,216,206,207,208,209,210,211,212,213,214,215,216,216,216
};

#elif 1

// 14-LED mode.
// Vest dimensions
#define MATRIX_WIDTH 15
#define MATRIX_HEIGHT 16
#define LEDNUM 215 // == LAST_VISIBLE_LED, total number of physical leds

const int pixelMap[] = {
   216,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10,216,216,216,
    11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24,216,
    25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38,216,
   216,216, 39, 40, 41, 42, 43, 44, 45, 46, 47,216,216,216,216,
    48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61,216,
    62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75,216,
    76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89,216,
    90, 91, 92, 93, 94, 95, 96, 97, 98, 99,100,101,102,103,216,
   104,105,106,107,108,109,110,111,112,113,114,115,116,117,216,
   118,119,120,121,122,123,124,125,126,127,128,129,130,131,216,
   132,133,134,135,136,137,138,139,140,141,142,143,144,145,216,
   146,147,148,149,150,151,152,153,154,155,156,157,158,159,216,
   216,216,160,161,162,163,164,165,166,167,168,216,216,216,216,
   169,170,171,172,173,174,175,176,177,178,179,180,181,182,216,
   183,184,185,186,187,188,189,190,191,192,193,194,195,196,216,
   216,197,198,199,200,201,202,203,204,205,206,207,216,216,216
};

#else

// 300 LED Guppy.
// Vest dimensions
#define MATRIX_WIDTH 15
#define MATRIX_HEIGHT 20
#define LEDNUM 300 // == LAST_VISIBLE_LED, total number of physical leds

const int pixelMap[] = {
   215,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10,216,217,218,
    11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24,219,
    25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38,220,
   222,214, 39, 40, 41, 42, 43, 44, 45, 46, 47,208,209,210,221,
    48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61,223,
    62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75,224,
    76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89,225,
    90, 91, 92, 93, 94, 95, 96, 97, 98, 99,100,101,102,103,226,
   104,105,106,107,108,109,110,111,112,113,114,115,116,117,227,
   118,119,120,121,122,123,124,125,126,127,128,129,130,131,228,
   132,133,134,135,136,137,138,139,140,141,142,143,144,145,229,
   146,147,148,149,150,151,152,153,154,155,156,157,158,159,230,
   232,239,160,161,162,163,164,165,166,167,168,211,212,213,231,
   169,170,171,172,173,174,175,176,177,178,179,180,181,182,233,
   183,184,185,186,187,188,189,190,191,192,193,194,195,196,234,
   235,197,198,199,200,201,202,203,204,205,206,207,236,237,238,
   240,241,242,243,244,245,246,247,248,249,250,251,252,253,254,
   255,256,257,258,259,260,261,262,263,264,265,266,267,268,269,
   270,271,272,273,274,275,276,277,278,279,280,281,282,283,284,
   285,286,287,288,289,270,271,272,273,274,275,276,277,278,279,
};

#endif

struct VestPixels : public PixelMap {
  VestPixels() : PixelMap(MATRIX_WIDTH, MATRIX_HEIGHT, LEDNUM, pixelMap, true) {
  }

  void doSetColor(int i,RgbaColor color) final {
    leds[i] = CRGB(color.red, color.green, color.blue);       
  } 

  CRGB leds[MATRIX_WIDTH*MATRIX_HEIGHT]; 
  
} pixels;

unsigned long buttonEvents[NUMBUTTONS];
uint8_t buttonStatuses[NUMBUTTONS];


Esp8266Network network("FISHLIGHT", "155155155");
//Esp8266Network network("detour", "thebeatenpath");
NetworkPlayer player(pixels, network);

void updateButtons(uint32_t currentMillis) {
  for (int i = 0; i < NUMBUTTONS; i++) {
    switch (buttonStatuses[i]) {
      case BTN_IDLE:
        if (digitalRead(buttonPins[i]) == LOW) {
          buttonEvents[i] = currentMillis;
          buttonStatuses[i] = BTN_DEBOUNCING;
        }
        break;

      case BTN_DEBOUNCING:
        if (currentMillis - buttonEvents[i] > BTN_DEBOUNCETIME) {
          if (digitalRead(buttonPins[i]) == LOW) {
            buttonStatuses[i] = BTN_PRESSED;
          }
        }
        break;

      case BTN_PRESSED:
        if (digitalRead(buttonPins[i]) == HIGH) {
          buttonStatuses[i] = BTN_RELEASED;
        } else if (currentMillis - buttonEvents[i] > BTN_LONGPRESSTIME) {
          buttonStatuses[i] = BTN_LONGPRESS;
        }
        break;

      case BTN_RELEASED:
        break;

      case BTN_LONGPRESS:
        break;

      case BTN_LONGPRESSREAD:
        if (digitalRead(buttonPins[i]) == HIGH) {
          buttonStatuses[i] = BTN_IDLE;
        }
        break;
    }
  }
}

uint8_t buttonStatus(uint8_t buttonNum) {

  uint8_t tempStatus = buttonStatuses[buttonNum];
  if (tempStatus == BTN_RELEASED) {
    buttonStatuses[buttonNum] = BTN_IDLE;
  } else if (tempStatus == BTN_LONGPRESS) {
    buttonStatuses[buttonNum] = BTN_LONGPRESSREAD;
  }
  return tempStatus;
}

void pushBrightness(void) {
  if (brightnessCursor >= sizeof(brightnessList) / sizeof(brightnessList[0])) {
    brightnessCursor = 0;
  }
  const uint8_t brightnessVal = brightnessList[brightnessCursor];
  FastLED.setBrightness(brightnessVal);
  info("Brightness set to %u", brightnessVal);
}

void doButtons(NetworkPlayer& player) {
  
  // Check the mode button (for switching between effects)
  switch (buttonStatus(0)) {
    case BTN_RELEASED:
      player.stopSpecial();
      player.next();
      break;

    case BTN_LONGPRESS: 
      break;
  }

  // Check the brightness adjust button
  switch (buttonStatus(1)) {
    case BTN_RELEASED:
      brightnessCursor++;
      pushBrightness();
      break;

    case BTN_LONGPRESS: // button was held down for a while
      brightnessCursor = 0;
      pushBrightness();
      break;
  }

  // Check the wifi adjust button
  switch (buttonStatus(2)) {
    case BTN_RELEASED:
      player.stopSpecial();
      player.showStatus(!player.isShowingStatus());
      info("Toggled status, %d", player.isShowingStatus());
      break;

    case BTN_LONGPRESS: 
      break;
  }

  // Check the special button
  switch (buttonStatus(3)) {
    case BTN_RELEASED:
      info("SPECIAL!");
      player.handleSpecial();
      break;

    case BTN_LONGPRESS:
      info("SPECIAL LONG!");
      break;
  }
}

void setup() {
  logLevel = debugLevel;
  
  Serial.begin(115200);

  for (uint8_t i = 0 ; i < sizeof(buttonPins) / sizeof(buttonPins[0]) ; i++) {
    pinMode(buttonPins[i], INPUT_PULLUP);
  }

  // Write FastLED configuration data
  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(pixels.leds,
                                    sizeof(pixels.leds)/sizeof(*pixels.leds));
  pushBrightness();
}


void loop()
{
  uint32_t currentMillis = (uint32_t)millis();
  updateButtons(currentMillis); // read, debounce, and process the buttons
  doButtons(player); // perform actions based on button state
  network.poll();
  player.render();
  FastLED.show();
  delay(10);
  // FastLED.delay(10);
}
