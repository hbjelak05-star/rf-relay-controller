#ifndef BOARD_GPIO_H
#define BOARD_GPIO_H

void board_gpio_init(void);

void led_blue_set(bool state);
void led_green_set(bool state);
void led_red_set(bool state);

void relay_set(bool state);

#endif
