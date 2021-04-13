//   RGB Vest Demo Code
//
//   Use Version 3.0 or later https://github.com/FastLED/FastLED
//   ZIP file https://github.com/FastLED/FastLED/archive/master.zip
//

#if !defined(ESP8266) && !defined(ESP32)
#error "Unexpected board"
#endif

// fixes flickering: https://github.com/FastLED/FastLED/issues/306
#define FASTLED_ALLOW_INTERRUPTS 0

#ifdef ESP8266
// you need this line or the feather huzzah or LoLin nodecmu pins will not be mapped properly
#define FASTLED_ESP8266_RAW_PIN_ORDER
#endif // ESP8266

#include <FastLED.h>
#include <DFSparks.h>

using namespace dfsparks;

#define BUTTON_LOCK 0

#if defined(ESP32)
#define LED_PIN  26
#elif defined(ESP8266)
#define LED_PIN  5
#endif

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

#if CAMPSIGN

// Camp Sign

#define FIRST_BRIGHTNESS 255

#define MATRIX_WIDTH 30
#define MATRIX_HEIGHT 30
#define LEDNUM 900 // == LAST_VISIBLE_LED, total number of physical leds

const int pixelMap[] = {
      0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
     30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59,
     60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89,
     90, 91, 92, 93, 94, 95, 96, 97, 98, 99,100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,
    120,121,122,123,124,125,126,127,128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,143,144,145,146,147,148,149,
    150,151,152,153,154,155,156,157,158,159,160,161,162,163,164,165,166,167,168,169,170,171,172,173,174,175,176,177,178,179,
    180,181,182,183,184,185,186,187,188,189,190,191,192,193,194,195,196,197,198,199,200,201,202,203,204,205,206,207,208,209,
    210,211,212,213,214,215,216,217,218,219,220,221,222,223,224,225,226,227,228,229,230,231,232,233,234,235,236,237,238,239,
    240,241,242,243,244,245,246,247,248,249,250,251,252,253,254,255,256,257,258,259,260,261,262,263,264,265,266,267,268,269,
    270,271,272,273,274,275,276,277,278,279,280,281,282,283,284,285,286,287,288,289,290,291,292,293,294,295,296,297,298,299,
    300,301,302,303,304,305,306,307,308,309,310,311,312,313,314,315,316,317,318,319,320,321,322,323,324,325,326,327,328,329,
    330,331,332,333,334,335,336,337,338,339,340,341,342,343,344,345,346,347,348,349,350,351,352,353,354,355,356,357,358,359,
    360,361,362,363,364,365,366,367,368,369,370,371,372,373,374,375,376,377,378,379,380,381,382,383,384,385,386,387,388,389,
    390,391,392,393,394,395,396,397,398,399,400,401,402,403,404,405,406,407,408,409,410,411,412,413,414,415,416,417,418,419,
    420,421,422,423,424,425,426,427,428,429,430,431,432,433,434,435,436,437,438,439,440,441,442,443,444,445,446,447,448,449,
    450,451,452,453,454,455,456,457,458,459,460,461,462,463,464,465,466,467,468,469,470,471,472,473,474,475,476,477,478,479,
    480,481,482,483,484,485,486,487,488,489,490,491,492,493,494,495,496,497,498,499,500,501,502,503,504,505,506,507,508,509,
    510,511,512,513,514,515,516,517,518,519,520,521,522,523,524,525,526,527,528,529,530,531,532,533,534,535,536,537,538,539,
    540,541,542,543,544,545,546,547,548,549,550,551,552,553,554,555,556,557,558,559,560,561,562,563,564,565,566,567,568,569,
    570,571,572,573,574,575,576,577,578,579,580,581,582,583,584,585,586,587,588,589,590,591,592,593,594,595,596,597,598,599,
    600,601,602,603,604,605,606,607,608,609,610,611,612,613,614,615,616,617,618,619,620,621,622,623,624,625,626,627,628,629,
    630,631,632,633,634,635,636,637,638,639,640,641,642,643,644,645,646,647,648,649,650,651,652,653,654,655,656,657,658,659,
    660,661,662,663,664,665,666,667,668,669,670,671,672,673,674,675,676,677,678,679,680,681,682,683,684,685,686,687,688,689,
    690,691,692,693,694,695,696,697,698,699,700,701,702,703,704,705,706,707,708,709,710,711,712,713,714,715,716,717,718,719,
    720,721,722,723,724,725,726,727,728,729,730,731,732,733,734,735,736,737,738,739,740,741,742,743,744,745,746,747,748,749,
    750,751,752,753,754,755,756,757,758,759,760,761,762,763,764,765,766,767,768,769,770,771,772,773,774,775,776,777,778,779,
    780,781,782,783,784,785,786,787,788,789,790,791,792,793,794,795,796,797,798,799,800,801,802,803,804,805,806,807,808,809,
    810,811,812,813,814,815,816,817,818,819,820,821,822,823,824,825,826,827,828,829,830,831,832,833,834,835,836,837,838,839,
    840,841,842,843,844,845,846,847,848,849,850,851,852,853,854,855,856,857,858,859,860,861,862,863,864,865,866,867,868,869,
    870,871,872,873,874,875,876,877,878,879,880,881,882,883,884,885,886,887,888,889,890,891,892,893,894,895,896,897,898,899
};

#elif 1

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


// Global maximum brightness value, maximum 255
//#define MAXBRIGHTNESS 120 
//#define STARTBRIGHTNESS 40
//#define BRIGHTNESS_INCREASE 51

#ifndef BRIGHTER
#define BRIGHTER 1
#endif // BRIGHTER

// this ordering is silly but we need the highest to be last
// in case it blows the fuse in the USB battery pack

#if BRIGHTER
#  ifndef FIRST_BRIGHTNESS
#  define FIRST_BRIGHTNESS 64
#  endif // FIRST_BRIGHTNESS
const uint8_t brightnessList[] = {64, 96, 8, 16, 32, 128};
#else // BRIGHTER
#  ifndef FIRST_BRIGHTNESS
#  define FIRST_BRIGHTNESS 32
#  endif // FIRST_BRIGHTNESS
const uint8_t brightnessList[] = { FIRST_BRIGHTNESS, 64, 96, 8, 16, 128};
#endif // BRIGHTER

uint8_t brightnessCursor = 0;

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

void doButtons(NetworkPlayer& player, uint32_t currentMillis) {
  const uint8_t btn0 = buttonStatus(0);
  const uint8_t btn1 = buttonStatus(1);
  const uint8_t btn2 = buttonStatus(2);
  const uint8_t btn3 = buttonStatus(3);

#if BUTTON_LOCK
  static uint8_t buttonLockState = 0;
  static uint32_t lastUnlockTime = 0;

  if (buttonLockState == 4 && currentMillis > lastUnlockTime + 10000) {
    buttonLockState = 0;
  }
  
  if (buttonLockState == 0 && btn0 == BTN_RELEASED) {
    buttonLockState++;
    return;
  }
  if (buttonLockState == 1 && btn3 == BTN_RELEASED) {
    buttonLockState++;
    return;
  }
  if (buttonLockState == 2 && btn2 == BTN_RELEASED) {
    buttonLockState++;
    return;
  }
  if (buttonLockState == 3 && btn1 == BTN_RELEASED) {
    buttonLockState++;
    lastUnlockTime = currentMillis;
    return;
  }

  if (buttonLockState < 4) {
    return;
  }

  lastUnlockTime = currentMillis;
#endif // BUTTON_LOCK
 
  // Check the mode button (for switching between effects)
  switch (btn0) {
    case BTN_RELEASED:
      player.stopSpecial();
      player.next();
      player.cycleAll();
      break;

    case BTN_LONGPRESS: 
      break;
  }

  // Check the brightness adjust button
  switch (btn1) {
    case BTN_RELEASED:
      brightnessCursor++;
      pushBrightness();
      break;

    case BTN_LONGPRESS: // button was held down for a while
      brightnessCursor = 0;
      pushBrightness();
      break;
  }

  // Check the back button
  switch (btn2) {
    case BTN_RELEASED:
      player.stopSpecial();
      player.prev();
      player.loopOne();
      info("Back button has been hit");
      break;

    case BTN_LONGPRESS: 
      break;
  }

  // Check the special button
  switch (btn3) {
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
  doButtons(player, currentMillis); // perform actions based on button state
  network.poll();
  player.render();
  FastLED.show();
  delay(10);
  // FastLED.delay(10);
}
