#include "subLogic_queue.h"

uint8_t queueSize;
uint8_t queueHead;

#define MAXSIZE 5

packet packetArray[MAXSIZE];

void queue_popToBuffer(packet *packetBuffer) {

    if (queueSize == 0) {
        return;
    }

    memcpy(packetBuffer, &packetArray[queueHead], sizeof(packet));
    queueSize--;

    queueHead++;
    queueHead = queueHead % MAXSIZE;

    return;
}

bool queue_addFromBuffer(uint8_t *inputBuffer, bool importance) {

    if (queueSize == MAXSIZE) {
        return false;
    }

    uint8_t index = (queueHead + queueSize) % MAXSIZE;

    memcpy(packetArray[index].data, inputBuffer, 28);
    packetArray[index].important = importance;

    queueSize++;
    return true;
}

bool queue_priorityAddFromBuffer(uint8_t *inputBuffer, bool importance) {
    if (queueSize == MAXSIZE) {
        return false;
    }

    uint8_t index = (queueHead + MAXSIZE - 1) % MAXSIZE;

    memcpy(packetArray[index].data, inputBuffer, 28);
    packetArray[index].important = importance;

    queueHead = index;
    queueSize++;
    return true;
}

uint8_t queue_getSize(void) { return queueSize; }

bool queue_isNotFull(void) { return (queueSize == MAXSIZE); }
