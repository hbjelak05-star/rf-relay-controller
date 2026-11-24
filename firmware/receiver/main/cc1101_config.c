#include "cc1101_config.h"
#include "cc1101.h"

#include "esp_log.h"
#include <stdint.h>

static const char *TAG = "CC1101_CONFIG";

typedef struct {
    uint8_t address;
    uint8_t value;
} cc1101_register_t;

static const cc1101_register_t cc1101_registers[] = {

    // GPIO configuration
    {0x00, 0x2E}, // IOCFG2
    {0x02, 0x2E}, // IOCFG0
    {0x03, 0x47}, // FIFOTHR

    // Packet configuration
    {0x04, 0xD3}, // SYNC1
    {0x05, 0x91}, // SYNC0
    {0x06, 0x3D}, // PKTLEN: 61 bytes
    {0x07, 0x00}, // PKTCTRL1
    {0x08, 0x04}, // PKTCTRL0: fixed length, CRC enabled

    // Frequency configuration
    {0x0B, 0x06}, // FSCTRL1
    {0x0C, 0x00}, // FSCTRL0
    {0x0D, 0x21}, // FREQ2
    {0x0E, 0x65}, // FREQ1
    {0x0F, 0x6A}, // FREQ0

    // Modem configuration
    {0x10, 0xC7}, // MDMCFG4
    {0x11, 0x83}, // MDMCFG3
    {0x12, 0x12}, // MDMCFG2: GFSK, 16/16 sync word
    {0x13, 0x20}, // MDMCFG1
    {0x14, 0xF8}, // MDMCFG0
    {0x15, 0x40}, // DEVIATN

    // Radio state machine
    {0x17, 0x00}, // MCSM1
    {0x18, 0x18}, // MCSM0

};

esp_err_t cc1101_configure(void)
{
    ESP_LOGI(TAG, "Configuring CC1101...");

    for (unsigned int i = 0;
         i < sizeof(cc1101_registers) / sizeof(cc1101_registers[0]);
         i++) {

        esp_err_t ret = cc1101_write_register(
            cc1101_registers[i].address,
            cc1101_registers[i].value
        );

        if (ret != ESP_OK) {
            ESP_LOGE(TAG,
                     "Failed to write register 0x%02X",
                     cc1101_registers[i].address);

            return ret;
        }
    }

    ESP_LOGI(TAG, "CC1101 configuration completed");

    return ESP_OK;
}
