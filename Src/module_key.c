#include "module_key.h"

uint16_t keyDownTime;
bool keyPressed;

enum KeyState { KEY_UndoFactoryReset, KEY_FactoryReset, KEY_Delete, KEY_Unlink, KEY_Link, KEY_None };
enum KeyState keyState;

void KEY_private_handleRelease(enum KeyState inputState) {
    switch (inputState) {
    case KEY_UndoFactoryReset:
        break;
    case KEY_FactoryReset:
        DEVICE_factoryReset();
        break;
    case KEY_Delete:
        break;
    case KEY_Unlink:
        DEVICE_startUnlinkingMode();
        break;
    case KEY_Link:
        DEVICE_startLinkingMode();
        break;
    case KEY_None:
        LED_setMode(Regular);
        break;
    }
}

void KEY_private_detectState() {
    if (keyDownTime >= (FACTORY_RESET_PRESS_TIME + 250)) {
        if (keyState != KEY_UndoFactoryReset) {
            keyState = KEY_UndoFactoryReset;
            LED_setMode(Regular);
        }
    } else if (keyDownTime >= FACTORY_RESET_PRESS_TIME && keyDownTime < (FACTORY_RESET_PRESS_TIME + 250)) {
        if (keyState != KEY_FactoryReset) {
            keyState = KEY_FactoryReset;
            LED_setMode(FactoryReset);
        }

    } else if (keyDownTime >= DELETE_KEY_PRESS_TIME && keyDownTime < FACTORY_RESET_PRESS_TIME) {
        if (keyState != KEY_Delete) {
            keyState = KEY_Delete;
            LED_setMode(Regular);
        }

    } else if (keyDownTime >= UNLINK_PRESS_TIME && keyDownTime < DELETE_KEY_PRESS_TIME) {
        if (keyState != KEY_Unlink) {
            keyState = KEY_Unlink;
            LED_setMode(Unlink);
        }

    } else if (keyDownTime >= LINK_PRESS_TIME && keyDownTime < UNLINK_PRESS_TIME) {
        if (keyState != KEY_Link) {
            keyState = KEY_Link;
            LED_setMode(Link);
        }
    } else {
        if (keyState != KEY_None) {
            keyState = KEY_None;
            LED_setMode(Regular);
        }
    }
}

void KEY_Loop() {

    if (keyDownTime == 0 && keyPressed == 0) {
        return;
    }

    if (keyPressed) {
        keyDownTime++;
        KEY_private_detectState();
    } else {
        keyDownTime = 0;
        KEY_private_handleRelease(keyState);
        keyState = KEY_None;
    }
}

void KEY_Interrupt() {
    GPIO_PinState temp = HAL_GPIO_ReadPin(KEY_PORT, KEY_PIN);

    if (temp == GPIO_PIN_SET) {
        keyPressed = true;
    } else {
        keyPressed = false;
    }
}
