#ifndef __UTIL_FLAG__
#define __UTIL_FLAG__

#include "UTIL_STD.h"

// flag structure
typedef union {
    struct {
        unsigned int Bit0 : 1;  // Bit 0
        unsigned int Bit1 : 1;  // Bit 1
        unsigned int Bit2 : 1;  // Bit 2
        unsigned int Bit3 : 1;  // Bit 3
        unsigned int Bit4 : 1;  // Bit 4
        unsigned int Bit5 : 1;  // Bit 5
        unsigned int Bit6 : 1;  // Bit 6
        unsigned int Bit7 : 1;  // Bit 7
        unsigned int Bit8 : 1;  // Bit 8
        unsigned int Bit9 : 1;  // Bit 9
        unsigned int Bit10 : 1; // Bit 10
        unsigned int Bit11 : 1; // Bit 11
        unsigned int Bit12 : 1; // Bit 12
        unsigned int Bit13 : 1; // Bit 13
        unsigned int Bit14 : 1; // Bit 14
        unsigned int Bit15 : 1; // Bit 15
        unsigned int Bit16 : 1; // Bit 16
        unsigned int Bit17 : 1; // Bit 17
        unsigned int Bit18 : 1; // Bit 18
        unsigned int Bit19 : 1; // Bit 19
        unsigned int Bit20 : 1; // Bit 20
        unsigned int Bit21 : 1; // Bit 21
        unsigned int Bit22 : 1; // Bit 22
        unsigned int Bit23 : 1; // Bit 23
        unsigned int Bit24 : 1; // Bit 24
        unsigned int Bit25 : 1; // Bit 25
        unsigned int Bit26 : 1; // Bit 26
        unsigned int Bit27 : 1; // Bit 27
        unsigned int Bit28 : 1; // Bit 28
        unsigned int Bit29 : 1; // Bit 29
        unsigned int Bit30 : 1; // Bit 30
        unsigned int Bit31 : 1; // Bit 31
    } flag;
    // union, so its either flags or all_flags
    unsigned int all_flags;
} _SysFlags;
extern volatile _SysFlags Flags;

#define FLAGRxMsgReceived Flags.flag.Bit0
#define FLAGTxComplete Flags.flag.Bit1
#define FLAGNormalMode Flags.flag.Bit2
#define FLAGUpdateMode Flags.flag.Bit3
#define FLAGSequenceProcessed Flags.flag.Bit4
#define FLAGIAPMode Flags.flag.Bit5

void FLAG_waitFlagHigh(unsigned int flag);
void FLAG_waitFlagLow(unsigned int flag);

#endif