#ifndef __LOGIC_IAP__
#define __LOGIC_IAP__

#include "UTIL_BASE.h"
#include "stm32l1xx_hal.h"
#include "stm32l1xx_hal_conf.h"
#include "subLogic_formatter.h"
#include "subLogic_queue.h"

void IAP_init();

void IAP_Loop();

void IAP_process(uint8_t *response);

void IAP_startIAP(uint8_t *sizeBuffer, uint8_t *checksumBuffer);

#endif