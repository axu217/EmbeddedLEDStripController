#include "UTIL_delay.h"

void DELAY_100us(void) {
    uint32_t i = 600;
    while (i--)
        ;
}

void DELAY_300us(void) {
    uint32_t i = 1600;
    while (i--)
        ;
}

void delay_1ms(void) {
    uint32_t i = 5350;
    while (i--)
        ;
}

void DELAY_nms(uint32_t ms) {
    uint32_t i = ms;
    while (i--)
        delay_1ms();
}
