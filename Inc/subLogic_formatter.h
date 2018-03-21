#ifndef __SUBLOGIC_FORMATTER__
#define __SUBLOGIC_FORMATTER__

#include "UTIL_BASE.h"

#include "logic_device.h"

#define POWER_ON_CMD 0xFFFF

void FORMATTER_genericFormat(uint8_t *input, uint8_t flag, uint16_t command, uint8_t *data,
                             uint8_t dataLength);
void FORMATTER_getFirmware(uint8_t *input, int8_t highByte, uint8_t lowByte);

#endif