#ifndef BOARD_GPIO_H
#define BOARD_GPIO_H

#include "esp_err.h"

esp_err_t board_adc_init(void);

esp_err_t ntc_read_temperature(float *temperature_c);

#endif
