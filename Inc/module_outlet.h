#ifndef __MODULE_OUTLET_
#define __MODULE_OUTLET_

#include "UTIL_BASE.h"

bool OUTLET_getShouldBeOn();
void OUTLET_Loop();
void OUTLET_setShouldBeOn(bool input);

#endif