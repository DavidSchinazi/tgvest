//
//  playground.cpp
//  DFSparks
//
//  Created by David Schinazi on 7/2/17.
//  Copyright © 2017 DiscoFish. All rights reserved.
//

#include "playground.hpp"
#include "Arduino.h"

void setup() {
  Serial.begin(115200);
}

static void log(const char *format, ...) __attribute__ ((format (printf, 1, 2)));

static void log(const char *format, ...) {
  static char logStr[4096];
  va_list ap;
  va_start(ap, format);
  const int res = vsnprintf(logStr, sizeof(logStr), format, ap);
  va_end(ap);
  if (res > 0) {
    Serial.print(logStr);
  }
}

static uint64_t myCounter = 0;

static const uint64_t sampleWindowLenMs = 10;

static uint16_t peakToPeakHistory[256] = {0};
static uint16_t peakToPeakIndex = 0;

void loop() {
  myCounter++;
  log("myCounter %lu\n", myCounter);

  const uint64_t windowStartMillis = millis();

  uint16_t windowMax = 0;
  uint16_t windowMin = 1024;

  uint64_t numSamples = 0;

  while (millis() - windowStartMillis < sampleWindowLenMs) {
    const uint32_t sample32 = analogRead(0);
    if (sample32 < 1024) { // toss out spurious readings
      const uint16_t sample = (uint16_t)sample32;
      numSamples++;
      if (sample > windowMax) {
        windowMax = sample;
      } else if (sample < windowMin) {
        windowMin = sample;
      }
    }
  }

  uint16_t peakToPeak = 0;
  if (windowMax > windowMin) {
    peakToPeak = windowMax - windowMin;
  }

  log("%lu a peak to peak %u numSamples %lu p2pI %u\n",
      myCounter, (uint32_t)peakToPeak, numSamples, (uint32_t)peakToPeakIndex);

  log("myCounter %p windowStartMillis %p\n", &myCounter, &windowStartMillis);

  peakToPeakHistory[peakToPeakIndex] = peakToPeak;

  peakToPeakIndex++;
  if (peakToPeakIndex >= sizeof(peakToPeakHistory)) {
    log("resetting p2p index\n");
    peakToPeakIndex = 0;
  }

}

