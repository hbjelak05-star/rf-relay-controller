#include <stdbool.h>
#include "driver/gpio.h"
#include "board_pins.h"
#include "board_gpio.h"

void board_gpio_init(void){
    gpio_set_direction(LED_BLUE, GPIO_MODE_OUTPUT);
    gpio_set_direction(LED_GREEN, GPIO_MODE_OUTPUT);
    gpio_set_direction(LED_RED, GPIO_MODE_OUTPUT);

    gpio_set_direction(RELAY, GPIO_MODE_OUTPUT);

    gpio_set_direction(BUTTON, GPIO_MODE_INPUT);

    gpio_set_direction(NTC, GPIO_MODE_INPUT);

    gpio_set_level(LED_BLUE, 0);
    gpio_set_level(LED_RED, 0);
    gpio_set_level(LED_GREEN, 0);
}

void led_blue_set(bool state){
    gpio_set_level(LED_BLUE, state);
}
void led_green_set(bool state){
    gpio_set_level(LED_GREEN, state);
}
void led_red_set(bool state){
    gpio_set_level(LED_RED, state);
}

void relay_set(bool state){
    gpio_set_level(RELAY, state);
}

bool button_read(void){
    return gpio_get_level(BUTTON);
}


