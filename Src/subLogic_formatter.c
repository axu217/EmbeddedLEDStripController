#include "subLogic_formatter.h"

// Definitions

void FORMATTER_getFirmware(uint8_t *input, int8_t highByte, uint8_t lowByte) {
    uint8_t returnHolder[28];
    memcpy(returnHolder, DEVICE_getID(), 4);
    memcpy(&returnHolder[4], DEVICE_getHubID(), 4);
    returnHolder[8] = 0x00;
    returnHolder[9] = 0xF1;
    returnHolder[10] = highByte;
    returnHolder[11] = lowByte;
    memcpy(input, returnHolder, 28);
}

void FORMATTER_genericFormat(uint8_t *input, uint8_t flag, uint16_t command, uint8_t *data,
                             uint8_t dataLength) {
    uint8_t returnHolder[28];
    memcpy(returnHolder, DEVICE_getID(), 4);
    memcpy(&returnHolder[4], DEVICE_getHubID(), 4);
    returnHolder[8] = flag;
    returnHolder[9] = (uint8_t) command >> 8;
    returnHolder[10] = (uint8_t) command & 0x00FF;

    uint8_t index;
    for (index = 0; index < dataLength; index++) {
        returnHolder[11 + index] = data[index];
    }

    memcpy(input, returnHolder, 28);
}
