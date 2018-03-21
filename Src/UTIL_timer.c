#include "UTIL_timer.h"

uint8_t timeCounter = 0;

bool twentyMsCounter = true;

void TIMER_interrupt() { twentyMsCounter = true; }

void TIMER_set20msCounter(bool input) { twentyMsCounter = input; }

bool TIMER_get20msCounter() { return twentyMsCounter; }

uint16_t TIMER_getRandomTime(void) {
    uint16_t random = 0;
    srand(*((uint32_t *) IDADDRESS));
    random = rand() % 100 + 1;
    return random;
}

int TIMER_getWeekDay(unsigned int iYear, unsigned int iMonth, unsigned int iDay) {
    int iWeek = 0;
    unsigned int y = 0, c = 0, m = 0, d = 0;
    if (iMonth == 1 || iMonth == 2) {
        c = (iYear - 1) / 100;
        y = (iYear - 1) % 100;
        m = iMonth + 12;
        d = iDay;
    } else {
        c = iYear / 100;
        y = iYear % 100;
        m = iMonth;
        d = iDay;
    }

    iWeek = y + y / 4 + c / 4 - 2 * c + 26 * (m + 1) / 10 + d - 1;
    iWeek = (iWeek >= 0) ? (iWeek % 7) : (iWeek % 7 + 7);
    if (iWeek == 0) {
        iWeek = 7;
    }
    return iWeek;
}