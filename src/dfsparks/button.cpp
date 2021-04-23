#include <DFSparks.h>

#include <Arduino.h>
#include <cstdint>

namespace dfsparks {

#ifndef BUTTON_LOCK
// If you want to modify this, change the value in the config.h header.
#  define BUTTON_LOCK 0
#endif // BUTTON_LOCK

#ifndef ATOM_MATRIX_SCREEN
#  ifdef ESP32
#    define ATOM_MATRIX_SCREEN 1
#  else // ESP32
#    define ATOM_MATRIX_SCREEN 0
#  endif // ESP32
#endif // ATOM_MATRIX_SCREEN

#ifdef ATOM_MATRIX_SCREEN
#define ATOM_SCREEN_NUM_LEDS 25
CRGB atomScreenLEDs[ATOM_SCREEN_NUM_LEDS] = {};
#endif // ATOM_MATRIX_SCREEN

#if defined(ESP32)
#define NUMBUTTONS 1
uint8_t buttonPins[NUMBUTTONS] = {39};
#elif defined(ESP8266)
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
#endif // ESPxx

#define BTN_IDLE 0
#define BTN_DEBOUNCING 1
#define BTN_PRESSED 2
#define BTN_RELEASED 3
#define BTN_LONGPRESS 4
#define BTN_LONGPRESSREAD 5

#define BTN_DEBOUNCETIME 20
#define BTN_LONGPRESSTIME 1000

unsigned long buttonEvents[NUMBUTTONS];
uint8_t buttonStatuses[NUMBUTTONS];

static constexpr uint32_t lockDelay = 10000;

uint8_t brightnessCursor = 0;

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


#if ATOM_MATRIX_SCREEN

enum atomMenuMode {
    kNext,
    kPrevious,
    kBrightness,
    kSpecial,
};

atomMenuMode menuMode = kNext;

void nextMode(NetworkPlayer& /*player*/, uint32_t /*currentMillis*/) {
  switch (menuMode) {
    case kNext: menuMode = kPrevious; break;
    case kPrevious: menuMode = kBrightness; break;
    case kBrightness: menuMode = kSpecial; break;
    case kSpecial: menuMode = kNext; break;
  };
}

void modeAct(NetworkPlayer& player, uint32_t /*currentMillis*/) {
  switch (menuMode) {
    case kNext:
      player.stopSpecial();
      player.next();
      player.cycleAll();
      break;
    case kPrevious:
      player.stopSpecial();
      player.prev();
      player.loopOne();
      info("Back button has been hit");
      break;
    case kBrightness:
      brightnessCursor++;
      pushBrightness();
      break;
    case kSpecial:
      info("SPECIAL!");
      player.handleSpecial();
      break;
  };

}

static const CRGB nextColor = CRGB::Blue;
static const CRGB menuIconNext[ATOM_SCREEN_NUM_LEDS] = {
    nextColor, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black,
    nextColor, nextColor,   CRGB::Black, CRGB::Black, CRGB::Black,
    nextColor, nextColor,   nextColor,   CRGB::Black, CRGB::Black,
    nextColor, nextColor,   CRGB::Black, CRGB::Black, CRGB::Black,
    nextColor, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black,
};

static const CRGB prevColor = CRGB::Red;
static const CRGB menuIconPrevious[ATOM_SCREEN_NUM_LEDS] = {
    CRGB::Black, CRGB::Black, prevColor, CRGB::Black, CRGB::Black,
    CRGB::Black, prevColor,   prevColor, CRGB::Black, CRGB::Black,
    prevColor,   prevColor,   prevColor, CRGB::Black, CRGB::Black,
    CRGB::Black, prevColor,   prevColor, CRGB::Black, CRGB::Black,
    CRGB::Black, CRGB::Black, prevColor, CRGB::Black, CRGB::Black,
};

static const CRGB menuIconBrightness[ATOM_SCREEN_NUM_LEDS] = {
    CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black,
    CRGB::DarkGoldenrod, CRGB::Goldenrod, CRGB::DarkGoldenrod, CRGB::Black, CRGB::Black,
    CRGB::Goldenrod, CRGB::LightGoldenrodYellow, CRGB::Goldenrod, CRGB::Black, CRGB::Black,
    CRGB::DarkGoldenrod, CRGB::Goldenrod, CRGB::DarkGoldenrod, CRGB::Black, CRGB::Black,
    CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black,
};

static const CRGB specColor = CRGB::Green;
static const CRGB menuIconSpecial[ATOM_SCREEN_NUM_LEDS] = {
    specColor,   specColor,   specColor,   CRGB::Black, CRGB::Black,
    CRGB::Black, CRGB::Black, specColor,   CRGB::Black, CRGB::Black,
    CRGB::Black, specColor,   CRGB::Black, CRGB::Black, CRGB::Black,
    CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black,
    CRGB::Black, specColor,   CRGB::Black, CRGB::Black, CRGB::Black,
};

void atomScreenUnlocked(NetworkPlayer& player, uint32_t /*currentMillis*/) {
  const CRGB* icon = atomScreenLEDs;
  switch (menuMode) {
    case kNext: icon = menuIconNext; break;
    case kPrevious: icon = menuIconPrevious; break;
    case kBrightness: icon = menuIconBrightness; break;
    case kSpecial: icon = menuIconSpecial; break;
  };
  for (int i = 0; i < ATOM_SCREEN_NUM_LEDS; i++) {
    atomScreenLEDs[i] = icon[i];
  }

  // Change top-right Atom matrix screen LED based on network status.
  CRGB networkColor = CRGB::Blue;
  switch (player.network().status()) {
    case Network::disconnected: networkColor = CRGB::Purple; break;
    case Network::connecting: networkColor = CRGB::Yellow; break;
    case Network::connected: networkColor = CRGB::Green; break;
    case Network::disconnecting: networkColor = CRGB::Orange; break;
    case Network::connection_failed: networkColor = CRGB::Red; break;
  }
  atomScreenLEDs[4] = networkColor;
}

void atomScreenClear() {
  for (int i = 0; i < ATOM_SCREEN_NUM_LEDS; i++) {
    atomScreenLEDs[i] = CRGB::Black;
  }
}

void atomScreenLong() {
  atomScreenClear();
  for (int i : {0,5,10,15,20,21,22}) {
    atomScreenLEDs[i] = CRGB::Gold;
  }
}

void atomScreenShort() {
  atomScreenClear();
  for (int i : {2,1,0,5,10,11,12,17,22,21,20}) {
    atomScreenLEDs[i] = CRGB::Gold;
  }
}

CLEDController* atomMatrixScreenController = nullptr;

#endif // ATOM_MATRIX_SCREEN


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

void setupButtons() {
  for (uint8_t i = 0 ; i < sizeof(buttonPins) / sizeof(buttonPins[0]) ; i++) {
    pinMode(buttonPins[i], INPUT_PULLUP);
  }
#if ATOM_MATRIX_SCREEN
  atomMatrixScreenController = &FastLED.addLeds<WS2812, /*DATA_PIN=*/27, GRB>(atomScreenLEDs,
                                                          ATOM_SCREEN_NUM_LEDS);
  atomScreenClear();
#endif // ATOM_MATRIX_SCREEN
}

void doButtons(NetworkPlayer& player, uint32_t currentMillis) {
#if defined(ESP32)
  const uint8_t btn = buttonStatus(0);
#if BUTTON_LOCK
  static uint8_t buttonLockState = 0;
  static uint32_t lastButtonTime = 0;

  if (buttonLockState != 0 && currentMillis > lastButtonTime + lockDelay) {
    buttonLockState = 0;
  }
  if (btn == BTN_RELEASED ||  btn == BTN_LONGPRESS) {
    lastButtonTime = currentMillis;

    if (buttonLockState == 0) {
      buttonLockState++;
    } else if (buttonLockState == 1) {
      if (btn == BTN_LONGPRESS) {
        buttonLockState++;
      } else {
        buttonLockState = 0;
      }
    } else if (buttonLockState == 2) {
      if (btn == BTN_RELEASED) {
        buttonLockState++;
      } else {
        buttonLockState = 0;
      }
    } else if (buttonLockState == 3) {
      if (btn == BTN_LONGPRESS) {
        buttonLockState++;
      } else {
        buttonLockState = 0;
      }
    }
  }

  if (buttonLockState == 0) {
    atomScreenClear();
  } else if ((buttonLockState % 2) == 1) {
    atomScreenLong();
  } else if (buttonLockState == 2) {
    atomScreenShort();
  }

  if (buttonLockState < 4) {
    return;
  }
#endif // BUTTON_LOCK
#if ATOM_MATRIX_SCREEN
#endif // ATOM_MATRIX_SCREEN
  switch (btn) {
    case BTN_RELEASED:
#if ATOM_MATRIX_SCREEN
    modeAct(player, currentMillis);
#endif // ATOM_MATRIX_SCREEN
      break;

    case BTN_LONGPRESS:
#if ATOM_MATRIX_SCREEN
    nextMode(player, currentMillis);
#endif // ATOM_MATRIX_SCREEN
      break;
  }
#if ATOM_MATRIX_SCREEN
  atomScreenUnlocked(player, currentMillis);
#endif // ATOM_MATRIX_SCREEN
#elif defined(ESP8266)
  const uint8_t btn0 = buttonStatus(0);
  const uint8_t btn1 = buttonStatus(1);
  const uint8_t btn2 = buttonStatus(2);
  const uint8_t btn3 = buttonStatus(3);

#if BUTTON_LOCK
  static uint8_t buttonLockState = 0;
  static uint32_t lastUnlockTime = 0;

  if (buttonLockState == 4 && currentMillis > lastUnlockTime + lockDelay) {
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
#endif // ESPxx
#if ATOM_MATRIX_SCREEN
  // M5Stack recommends not setting the atom screen brightness greater
  // than 20 to avoid melting the screen/cover over the LEDs.
  atomMatrixScreenController->showLeds(20);
#endif // ATOM_MATRIX_SCREEN
}

uint8_t brightnessVal = FIRST_BRIGHTNESS;

void pushBrightness(void) {
  if (brightnessCursor >= sizeof(brightnessList) / sizeof(brightnessList[0])) {
    brightnessCursor = 0;
  }
  brightnessVal = brightnessList[brightnessCursor];
  info("Brightness set to %u", brightnessVal);
}

uint8_t getBrightness() {
  return brightnessVal;
}

} // namespace dfsparks
