#include "UTIL_flag.h"

volatile _SysFlags Flags;

void FLAG_waitFlagHigh(unsigned int flag) {
    while (flag != 0b1) {
    }
    return;
}

void FLAG_waitFlagLow(unsigned int flag) {
    while (flag != 0b0) {
    }
    return;
}