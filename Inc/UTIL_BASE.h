#ifndef __UTIL_BASE__
#define __UTIL_BASE__

#include "UTIL_STD.h"
#include "UTIL_constant.h"
#include "UTIL_constant_program.h"
#include "UTIL_delay.h"
#include "UTIL_eeprom.h"
#include "UTIL_flag.h"
#include "UTIL_timer.h"

typedef struct packet {
    uint8_t data[28];
    bool important;
} packet;

#endif