#include "dfsparks/vest.h"

#ifdef DFSPARKS_PLATFORMIO

void setup() {
  dfsparks::vestSetup();
}

void loop() {
  dfsparks::vestLoop();
}

#endif // DFSPARKS_PLATFORMIO
