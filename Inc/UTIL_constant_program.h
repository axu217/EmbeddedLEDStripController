#ifndef UTIL_constant_program
#define UTIL_constant_program

#include "UTIL_BASE.h"

#define LED_RED_PORT GPIOB
#define LED_RED_PIN GPIO_PIN_4
#define LED_GREEN_PORT GPIOB
#define LED_GREEN_PIN GPIO_PIN_3

#define NSS_PIN GPIO_PIN_4
#define NSS_GPIO_PORT GPIOA

#define KEY_PORT GPIOB
#define KEY_PIN GPIO_PIN_0

#define DIO0_PORT GPIOA
#define DIO0_PIN GPIO_PIN_12
#define DIO1_PORT GPIOA
#define DIO1_PIN GPIO_PIN_11

#define FLAG_broadcast 0x80
#define FLAG_direct 0x00
#define FLAG_acknowledge 0x20

#define OUTLET_PORT GPIOB
#define OUTLET_PIN GPIO_PIN_6

#define RST_PORT_GPIO_PORT GPIOA
#define RST_PORT_PIN GPIO_PIN_3
#define RX_GPIO_PORT GPIOA
#define RX_PIN GPIO_PIN_2
#define TX_GPIO_PORT GPIOA
#define TX_PIN GPIO_PIN_1

#endif