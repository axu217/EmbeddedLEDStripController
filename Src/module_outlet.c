#include "module_outlet.h"

bool outletShouldBeOn = false;
bool pastOutletStatus = false;

void OUTLET_private_turnOn() { HAL_GPIO_WritePin(OUTLET_PORT, OUTLET_PIN, GPIO_PIN_SET); }

void OUTLET_private_turnOff() { HAL_GPIO_WritePin(OUTLET_PORT, OUTLET_PIN, GPIO_PIN_RESET); }

bool OUTLET_getShouldBeOn() { return outletShouldBeOn; }

void OUTLET_setShouldBeOn(bool input) { outletShouldBeOn = input; }

void OUTLET_Loop() {

    if (outletShouldBeOn != pastOutletStatus) {
        pastOutletStatus = outletShouldBeOn;

        if (outletShouldBeOn) {
            OUTLET_private_turnOn();
        } else {
            OUTLET_private_turnOff();
        }
    }
}
