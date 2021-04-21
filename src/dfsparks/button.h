#ifndef DFSPARKS_BUTTON_H
#define DFSPARKS_BUTTON_H

#include "dfsparks/config.h"
#include "dfsparks/player.h"

namespace dfsparks {

void setupButtons();
void doButtons(NetworkPlayer& player, uint32_t currentMillis);
void updateButtons(uint32_t currentMillis);
void pushBrightness(void);

} // namespace dfsparks

#endif // DFSPARKS_BUTTON_H
