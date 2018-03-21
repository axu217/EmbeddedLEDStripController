#ifndef __UTIL_TIMER__
#define __UTIL_TIMER__

#include "UTIL_STD.h"
#include "UTIL_eeprom.h"

void TIMER_interrupt();

void TIMER_set20msCounter(bool input);

bool TIMER_get20msCounter();

uint16_t TIMER_getRandomTime(void);

int TIMER_getWeekDay(unsigned int iYear, unsigned int iMonth, unsigned int iDay);

#endif
