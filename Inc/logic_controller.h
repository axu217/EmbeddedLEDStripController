#ifndef __LOGIC_CONTROLLER__
#define __LOGIC_CONTROLLER__

#include "main.h"

// break

#include "UTIL_BASE.h"
#include "logic_device.h"
#include "logic_iap.h"
#include "logic_responder.h"
#include "logic_scheduler.h"
#include "main.h"
#include "module_key.h"
#include "module_led.h"
#include "module_outlet.h"
#include "subLogic_formatter.h"
#include "subLogic_queue.h"
#include "subLogic_sx1278lr.h"

void CONTROLLER_INIT();
void CONTROLLER_Loop();
void CONTROLLER_interruptDIO0();
void CONTROLLER_interruptDIO1();

typedef enum { Standby, RxWait, TxWait, CADWait, CADDone, TxDone, RxDone, RxTimeout } State;

typedef struct flags {
    // unsigned int CADDoneIRQ : 1;
    // unsigned int TxIRQ : 1;
    // unsigned int RxIRQ : 1;
    unsigned int CADDetectedIRQ : 1;
} LORA_Flags;

#endif