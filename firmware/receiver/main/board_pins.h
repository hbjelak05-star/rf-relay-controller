#ifndef BOARD_PINS_H
#define BOARD_PINS_H

#include "driver/gpio.h"

//CC1101 MODULE
#define CC1101_SCK      GPIO_NUM_18
#define CC1101_CSN      GPIO_NUM_5
#define CC1101_SI       GPIO_NUM_23
#define CC1101_GD0      GPIO_NUM_17
#define CC1101_GD01     GPIO_NUM_19
#define CC1101_GD02     GPIO_NUM_16

#define LED_BLUE      GPIO_NUM_33
#define LED_GREEN     GPIO_NUM_25
#define LED_RED       GPIO_NUM_26

#define BUTTON GPIO_NUM_27

#define NTC GPIO_NUM_36

#define RELAY GPIO_NUM_13

#endif
