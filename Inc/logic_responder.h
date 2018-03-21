#ifndef __LOGIC_RESPONDER__
#define __LOGIC_RESPONDER__

#include "UTIL_BASE.h"
#include "logic_device.h"
#include "logic_iap.h"
#include "logic_scheduler.h"
#include "stm32l1xx_hal_conf.h"
#include "subLogic_formatter.h" // FOR THE CONSTANTS
#include "subLogic_sx1278lr.h"

void RESPONDER_handleMessage();

void RESPONDER_private_handleDirectAck(uint8_t *rxBuffer);
void RESPONDER_private_handleDirect(uint8_t *rxBuffer);
void RESPONDER_private_handleBroadcast(uint8_t *rxBuffer);

#endif