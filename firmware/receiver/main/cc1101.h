
#ifndef CC1101_H
#define CC1101_H

#include "esp_err.h"
#include <stdint.h>

esp_err_t cc1101_init(void);

esp_err_t cc1101_read_id(
    uint8_t *part_number,
    uint8_t *version
);

esp_err_t cc1101_write_register(
    uint8_t address,
    uint8_t value
);

#endif
