#ifndef DFSPARKS_H
#define DFSPARKS_H

// Fixes flickering <https://github.com/FastLED/FastLED/issues/306>.
#define FASTLED_ALLOW_INTERRUPTS 0

#ifdef ESP8266
// Required to map feather huzzah and LoLin nodecmu pins properly.
#  define FASTLED_ESP8266_RAW_PIN_ORDER
#endif // ESP8266

#include <FastLED.h>

#include "dfsparks/board.h"
#include "dfsparks/button.h"
#include "dfsparks/color.h"
#include "dfsparks/config.h"
#include "dfsparks/player.h"
#include "dfsparks/log.h"
#include "dfsparks/networks/esp8266wifi.h"
#include "dfsparks/vest.h"

#endif /* DFSPARKS_H */
