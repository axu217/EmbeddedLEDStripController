#ifndef __MODULE_LED__
#define __MODULE_LED__

#include "UTIL_BASE.h"
#include "module_outlet.h"

enum LEDMode { Regular, Link, Unlink, FactoryReset, FirmwareUpdate };

void LED_setMode(enum LEDMode newMode);
void LED_Loop();

void LED_private_normalLoop();

void LED_private_linkLoop();

void LED_private_unlinkLoop();

void LED_private_factoryResetLoop();

void LED_private_firmwareUpdateLoop();

#endif