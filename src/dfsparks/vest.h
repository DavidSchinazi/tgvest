#ifndef DFSPARKS_VEST_H
#define DFSPARKS_VEST_H

#include "dfsparks/config.h"

// Fixes flickering <https://github.com/FastLED/FastLED/issues/306>.
#define FASTLED_ALLOW_INTERRUPTS 0

#ifdef ESP8266
// Required to map feather huzzah and LoLin nodecmu pins properly.
#  define FASTLED_ESP8266_RAW_PIN_ORDER
#endif // ESP8266

#include <FastLED.h>
#include <DFSparks.h>

namespace dfsparks {

void vestSetup(void);
void vestLoop(void);

} // namespace dfsparks

#endif // DFSPARKS_VEST_H
