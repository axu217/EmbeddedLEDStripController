#ifndef __LOGIC_SCHEDULER__
#define __LOGIC_SCHEDULER__

#include "UTIL_BASE.h"
#include "module_outlet.h"
#include "stm32l1xx_hal.h"
#include "stm32l1xx_hal_conf.h"

extern RTC_HandleTypeDef hrtc;

typedef struct SunTime {
    unsigned char hour;
    unsigned char minute;
} SunTime;

typedef struct Schedule {
    uint8_t weekMask;
    uint8_t zoneMask;
    uint8_t onHour;
    uint8_t onMinute;
    uint8_t offHour;
    uint8_t offMinute;
} Schedule;

void SCHEDULER_Loop();
void SCHEDULER_init();
void SCHEDULER_setSleepMinutes(uint16_t input);
void SCHEDULER_setWakeupMiniutes(uint16_t input);
uint16_t SCHEDULER_getSleepMinutes();
uint16_t SCHEDULER_getWakeupMinutes();

SunTime SCHEDULER_getSunrise();

SunTime SCHEDULER_getSunset();

void SCHEDULER_setSunset(SunTime input);

void SCHEDULER_setSunrise(SunTime input);

#endif