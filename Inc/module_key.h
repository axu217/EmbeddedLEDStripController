#ifndef __MODULE_KEY__
#define __MODULE_KEY__

#include "UTIL_BASE.h"
#include "logic_device.h"
#include "module_led.h"

#define LINK_PRESS_TIME 250
#define UNLINK_PRESS_TIME 500
#define DELETE_KEY_PRESS_TIME 750
#define FACTORY_RESET_PRESS_TIME 1000
#define LINK_UNLINK_TIMEOUT 3000

void KEY_Loop();
void KEY_Interrupt();

#endif