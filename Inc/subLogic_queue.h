#ifndef __SUBLOGIC_QUEUE__
#define __SUBLOGIC_QUEUE__

#include "UTIL_BASE.h"

void queue_popToBuffer(packet *packetBuffer);

bool queue_addFromBuffer(uint8_t *inputBuffer, bool importance);

bool queue_priorityAddFromBuffer(uint8_t *inputBuffer, bool importance);

uint8_t queue_getSize(void);

bool queue_isNotFull(void);

#endif