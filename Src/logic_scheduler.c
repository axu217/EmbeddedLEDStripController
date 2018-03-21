#include "logic_scheduler.h"

Schedule schedules[6];
uint16_t sleepMinutes;
uint16_t wakeupMinutes;
SunTime Sunrise, Sunset;
bool oneMinuteFlag = true;

// Check every minute?
void SCHEDULER_Loop() {

    if (oneMinuteFlag == false) {
        return;
    }
    oneMinuteFlag = false;

    RTC_TimeTypeDef tempTime;
    HAL_RTC_GetTime(&hrtc, &tempTime, RTC_FORMAT_BIN);

    RTC_DateTypeDef tempDay;
    HAL_RTC_GetDate(&hrtc, &tempDay, RTC_FORMAT_BIN);

    uint8_t weekday = tempDay.WeekDay - 1;
    uint8_t hour = tempTime.Hours;
    uint8_t minutes = tempTime.Minutes;

    for (uint8_t i = 0; i < 6; i++) {
        bool appliesToday = schedules[i].weekMask & (1 << weekday);

        if (appliesToday) {
            if (hour == schedules[i].onHour && minutes == schedules[i].onMinute) {
                OUTLET_setShouldBeOn(true);
            }

            if (hour == schedules[i].offHour && minutes == schedules[i].offMinute) {
                OUTLET_setShouldBeOn(false);
            }
        }
    }

    if (sleepMinutes > 0) {
        sleepMinutes--;
        if (sleepMinutes == 0) // Turn off the socket
        {
            __disable_irq();
            OUTLET_setShouldBeOn(false);
            __enable_irq();
        }
    }
    if (wakeupMinutes > 0) {
        wakeupMinutes--;
        if (wakeupMinutes == 0) // Turn on the socket
        {
            __disable_irq();
            OUTLET_setShouldBeOn(true);
            __enable_irq();
        }
    }
}

void SCHEDULER_init() {

    for (uint8_t i = 0; i < 6; i++) {
        uint8_t tempBuf[6];
        EEPROM_ReadNBytes(SCHEDULE_BASE, tempBuf, 6);

        schedules[i].weekMask = tempBuf[0];
        schedules[i].zoneMask = tempBuf[1];
        schedules[i].onHour = tempBuf[2];
        schedules[i].onMinute = tempBuf[3];
        schedules[i].offHour = tempBuf[4];
        schedules[i].offMinute = tempBuf[5];
    }

    Sunrise.hour = EEPROM_ReadByte(SUN_RISE_SET_ADDR);
    Sunrise.minute = EEPROM_ReadByte(SUN_RISE_SET_ADDR + 1);
    Sunset.hour = EEPROM_ReadByte(SUN_RISE_SET_ADDR + 2);
    Sunset.minute = EEPROM_ReadByte(SUN_RISE_SET_ADDR + 3);

    if (Sunrise.hour < 5) {
        Sunrise.hour = 5;
    }

    if (Sunrise.hour > 11) {
        Sunrise.hour = 11;
    }

    if (Sunrise.minute > 59) {
        Sunrise.minute = 0;
    }

    if (Sunset.hour < 16) {
        Sunrise.minute = 0;
    }

    if (Sunset.hour > 21) {
        Sunset.hour = 21;
    }

    if (Sunset.minute > 59) {
        Sunrise.minute = 0;
    }
}

void SCHEDULER_setSleepMinutes(uint16_t input) { sleepMinutes = input; }

void SCHEDULER_setWakeupMiniutes(uint16_t input) { wakeupMinutes = input; }

void HAL_RTCEx_WakeUpTimerEventCallback(RTC_HandleTypeDef *hrtc) { oneMinuteFlag = true; }

uint16_t SCHEDULER_getSleepMinutes() { return sleepMinutes; }

uint16_t SCHEDULER_getWakeupMinutes() { return wakeupMinutes; }

SunTime SCHEDULER_getSunrise() { return Sunrise; }

SunTime SCHEDULER_getSunset() { return Sunset; }

void SCHEDULER_setSunset(SunTime input) { Sunset = input; }

void SCHEDULER_setSunrise(SunTime input) { Sunrise = input; }