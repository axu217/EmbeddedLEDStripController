#include "logic_controller.h"

LORA_Flags flags;
State currentState = Standby;
uint16_t powerOnCommandRandomTime;
uint16_t requestTimeRandomTime;

bool waitForResponse = false;

void normalLoop(void);

void CONTROLLER_private_initialSendID() {
    if (powerOnCommandRandomTime > 0) {
        powerOnCommandRandomTime--;
        if (powerOnCommandRandomTime == 0) {
            uint8_t data[24];
            uint8_t payload[4];
            payload[0] = DEVICE_getHardwareVersion();
            payload[1] = DEVICE_getSoftwareVersion();
            payload[2] = DEVICE_getCategory();
            payload[3] = DEVICE_getSubcategory();
            FORMATTER_genericFormat(data, FLAG_broadcast, CMD_powerOn, payload, 4);
            queue_addFromBuffer(data, true);
        }
    }
}

void CONTROLLER_private_initialRequestTime() {
    if (requestTimeRandomTime > 0) {
        requestTimeRandomTime--;
        if (requestTimeRandomTime == 0) {
            uint8_t data[24];
            uint8_t payload[4];
            payload[0] = DEVICE_getHardwareVersion();
            payload[1] = DEVICE_getSoftwareVersion();
            payload[2] = DEVICE_getCategory();
            payload[3] = DEVICE_getSubcategory();
            FORMATTER_genericFormat(data, FLAG_broadcast, CMD_powerOn, payload, 4);
            queue_addFromBuffer(data, true);
        }
    }
}

void CONTROLLER_INIT() {

    DEVICE_init();
    SX1278LoraInit();
    IAP_init();
    powerOnCommandRandomTime = TIMER_getRandomTime();
    requestTimeRandomTime = powerOnCommandRandomTime + 25;
}

void CONTROLLER_Loop() {
    switch (currentState) {
    case Standby:
        if (waitForResponse != true && queue_getSize() != 0) {
            currentState = CADWait;
            SX1278LoRaStartCAD();
        } else {
            currentState = RxWait;
            SX1278LoRaStartRx();
        }
        break;
    case CADDone:
        if (flags.CADDetectedIRQ == 0) {
            packet currentPacket;
            queue_popToBuffer(&currentPacket);
            SX1278LoRaSetTxMessage(currentPacket.data);
            SX1278LoRaStartTx();
            if (currentPacket.important == true) {
                waitForResponse = true;
            }
            currentState = TxWait;
        } else {
            flags.CADDetectedIRQ = 0;
            SX1278LoRaStartCAD();
            currentState = CADWait;
        }
        break;
    case TxDone:
        currentState = Standby;
        break;
    case RxDone:
        RESPONDER_handleMessage();
        currentState = Standby;
        waitForResponse = false;
    case RxTimeout:
        currentState = Standby;
        waitForResponse = false;
    default:
        break;
    }
    normalLoop();
}

void normalLoop(void) {

    if (TIMER_get20msCounter() == true) {
        TIMER_set20msCounter(false);

        // ACTUALLY ONLY RUNS ONCE. LOOP DECREMENTS COUNTER
        CONTROLLER_private_initialSendID();
        CONTROLLER_private_initialRequestTime();

        MAIN_feedWatchdog();
        KEY_Loop();
        SCHEDULER_Loop();
        OUTLET_Loop();
        LED_Loop();
        IAP_Loop(); // Only does stuff if IAP_mode enabled
    }
}

void CONTROLLER_interruptDIO0() {

    switch (currentState) {
    case RxWait:
        currentState = RxDone;
        break;
    case TxWait:
        currentState = TxDone;
        break;
    case CADWait:
        currentState = CADDone;
        break;
    default:
        break;
    }
}

void CONTROLLER_interruptDIO1() {

    switch (currentState) {
    case RxWait:
        currentState = RxTimeout;
        break;
    case CADWait:
        currentState = CADDone;
        flags.CADDetectedIRQ = 1;
        break;
    case CADDone:
        flags.CADDetectedIRQ = 1;
        break;
    default:
        break;
    }
}
