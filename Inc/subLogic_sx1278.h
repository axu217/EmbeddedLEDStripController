#ifndef __SUBLOGIC_SX1278__
#define __SUBLOGIC_SX1278__

#include "UTIL_BASE.h"
#include "main.h"
#include "stm32l1xx_hal.h"

void sx1278_Reset(void);
void sx1278_WriteByte(uint8_t addr, uint8_t data);
void sx1278_WriteNbytes(uint8_t addr, uint8_t *buffer, uint8_t size);
void sx1278_ReadByte(uint8_t addr, uint8_t *buffer);
void sx1278_ReadNbytes(uint8_t addr, uint8_t *buffer, uint8_t size);
void sx1278_WriteRxTx(uint8_t enable);

#endif
