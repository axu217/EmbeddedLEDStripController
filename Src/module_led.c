#include "module_led.h"

uint16_t flashCount = 0;

enum LEDMode ledMode;

//
void LED_private_normalLoop();
void LED_private_linkLoop();
void LED_private_unlinkLoop();
void LED_private_factoryResetLoop();
void LED_private_firmwareUpdateLoop();

void LED_private_Green_On(void) { HAL_GPIO_WritePin(LED_GREEN_PORT, LED_GREEN_PIN, GPIO_PIN_SET); }

void LED_private_Green_Off(void) { HAL_GPIO_WritePin(LED_GREEN_PORT, LED_GREEN_PIN, GPIO_PIN_RESET); }

void LED_private_Red_On(void) { HAL_GPIO_WritePin(LED_RED_PORT, LED_RED_PIN, GPIO_PIN_SET); }

void LED_private_Red_Off(void) { HAL_GPIO_WritePin(LED_RED_PORT, LED_RED_PIN, GPIO_PIN_RESET); }

//

void LED_setMode(enum LEDMode newMode) {
    ledMode = newMode;
    flashCount = 0;
    LED_private_Green_Off();
    LED_private_Red_Off();
}

// ASSUMING ONE MILLISECOND SECOND LOOP
void LED_Loop() {

    switch (ledMode) {
    case Regular:
        LED_private_normalLoop();
        break;
    case Link:
        LED_private_linkLoop();
        break;
    case Unlink:
        LED_private_unlinkLoop();
        break;
    case FactoryReset:
        LED_private_factoryResetLoop();
        break;
    case FirmwareUpdate:
        LED_private_firmwareUpdateLoop();
        break;
    }
}

void LED_private_normalLoop() {

    if (OUTLET_getShouldBeOn() == true) {
        LED_private_Green_On();
        LED_private_Red_Off();
    } else {
        LED_private_Red_On();
        LED_private_Green_Off();
    }
}

void LED_private_linkLoop() {
    flashCount++;

    if (flashCount >= 100) {
        flashCount = 0;
    }

    if (flashCount < 50) {
        LED_private_Green_On();
    } else {
        LED_private_Green_Off();
    }
}

void LED_private_unlinkLoop() {
    flashCount++;

    if (flashCount >= 100) {
        flashCount = 0;
    }

    if (flashCount < 50) {
        LED_private_Red_On();
    } else {
        LED_private_Red_Off();
    }
}

void LED_private_factoryResetLoop() {
    flashCount++;

    if (flashCount >= 100) {
        flashCount = 0;
    }

    if (flashCount < 50) {
        LED_private_Red_On();
    } else {
        LED_private_Red_Off();
    }
}

void LED_private_firmwareUpdateLoop() {
    flashCount++;

    if (flashCount >= 100) {
        flashCount = 0;
    }

    if (flashCount < 50) {
        LED_private_Red_On();
    } else {
        LED_private_Red_Off();
    }
}
