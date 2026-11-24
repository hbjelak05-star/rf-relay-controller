
#include "cc1101.h"
#include "board_pins.h"

#include "driver/spi_master.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_rom_sys.h"

#define CC1101_SPI_HOST SPI2_HOST
#define CC1101_SPI_CLOCK_HZ 1000000

#define CC1101_SRES       0x30
#define CC1101_PARTNUM    0x30
#define CC1101_VERSION    0x31
#define CC1101_READ_SINGLE 0x80

#define CC1101_READY_TIMEOUT 10000

static const char *TAG = "CC1101";

static spi_device_handle_t cc1101_spi;

/* Wait until the CC1101 SO/MISO pin goes LOW */
static esp_err_t cc1101_wait_ready(void)
{
    for (int i = 0; i < CC1101_READY_TIMEOUT; i++) {

        if (gpio_get_level(CC1101_GD01) == 0) {
            return ESP_OK;
        }

        esp_rom_delay_us(10);
    }

    ESP_LOGE(TAG, "CC1101 ready timeout");

    return ESP_ERR_TIMEOUT;
}

/* Send one command byte while CSN is LOW */
static esp_err_t cc1101_send_command(uint8_t command)
{
    spi_transaction_t transaction = {
        .length = 8,
        .tx_buffer = &command
    };

    return spi_device_transmit(cc1101_spi, &transaction);
}

/* Reset the CC1101 */
static esp_err_t cc1101_reset(void)
{
    esp_err_t ret;

    gpio_set_level(CC1101_CSN, 1);
    esp_rom_delay_us(5000);

    gpio_set_level(CC1101_CSN, 0);

    ret = cc1101_wait_ready();

    if (ret != ESP_OK) {
        gpio_set_level(CC1101_CSN, 1);
        return ret;
    }

    ret = cc1101_send_command(CC1101_SRES);

    if (ret != ESP_OK) {
        gpio_set_level(CC1101_CSN, 1);
        return ret;
    }

    ret = cc1101_wait_ready();

    gpio_set_level(CC1101_CSN, 1);

    if (ret != ESP_OK) {
        return ret;
    }

    esp_rom_delay_us(1000);

    ESP_LOGI(TAG, "CC1101 reset completed");

    return ESP_OK;
}

/* Initialize SPI and the CC1101 */
esp_err_t cc1101_init(void)
{
    esp_err_t ret;

    gpio_reset_pin(CC1101_CSN);
    gpio_set_direction(CC1101_CSN, GPIO_MODE_OUTPUT);
    gpio_set_level(CC1101_CSN, 1);

    spi_bus_config_t bus_config = {
        .mosi_io_num = CC1101_SI,
        .miso_io_num = CC1101_GD01,
        .sclk_io_num = CC1101_SCK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = 32
    };

    ret = spi_bus_initialize(
        CC1101_SPI_HOST,
        &bus_config,
        SPI_DMA_DISABLED
    );

    if (ret != ESP_OK && ret != ESP_ERR_INVALID_STATE) {
        ESP_LOGE(TAG, "SPI bus initialization failed: %s",
                 esp_err_to_name(ret));
        return ret;
    }

    spi_device_interface_config_t device_config = {
        .clock_speed_hz = CC1101_SPI_CLOCK_HZ,
        .mode = 0,
        .spics_io_num = -1,
        .queue_size = 1
    };

    ret = spi_bus_add_device(
        CC1101_SPI_HOST,
        &device_config,
        &cc1101_spi
    );

    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "SPI device initialization failed: %s",
                 esp_err_to_name(ret));
        return ret;
    }

    ESP_LOGI(TAG, "SPI initialized");

    ret = cc1101_reset();

    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "CC1101 reset failed");
        return ret;
    }

    return ESP_OK;
}

/* Read a CC1101 configuration or identification register */
static esp_err_t cc1101_read_register(uint8_t address, uint8_t *value)
{
    if (value == NULL) {
        return ESP_ERR_INVALID_ARG;
    }

    esp_err_t ret;

    uint8_t tx_data[2] = {
        (uint8_t)(address | CC1101_READ_SINGLE),
        0x00
    };

    uint8_t rx_data[2] = {0};

    spi_transaction_t transaction = {
        .length = 16,
        .tx_buffer = tx_data,
        .rx_buffer = rx_data
    };

    gpio_set_level(CC1101_CSN, 0);

    ret = cc1101_wait_ready();

    if (ret == ESP_OK) {
        ret = spi_device_transmit(cc1101_spi, &transaction);
    }

    gpio_set_level(CC1101_CSN, 1);

    if (ret == ESP_OK) {
        *value = rx_data[1];
    }

    return ret;
}


esp_err_t cc1101_write_register(uint8_t address, uint8_t value)
{
    uint8_t tx_data[2] = {
        address,
        value
    };

    spi_transaction_t transaction = {
        .length = 16,
        .tx_buffer = tx_data
    };

    gpio_set_level(CC1101_CSN, 0);

    esp_err_t ret = cc1101_wait_ready();

    if (ret == ESP_OK) {
        ret = spi_device_transmit(cc1101_spi, &transaction);
    }

    gpio_set_level(CC1101_CSN, 1);

    return ret;
}


/* Read the CC1101 identification registers */
esp_err_t cc1101_read_id(uint8_t *part_number, uint8_t *version)
{
    if (part_number == NULL || version == NULL) {
        return ESP_ERR_INVALID_ARG;
    }

    esp_err_t ret;

    ret = cc1101_read_register(CC1101_PARTNUM, part_number);
    if (ret != ESP_OK) {
        return ret;
    }

    ret = cc1101_read_register(CC1101_VERSION, version);

    if (ret != ESP_OK) {
        return ret;
    }

    ESP_LOGI(TAG, "PARTNUM: 0x%02X", *part_number);
    ESP_LOGI(TAG, "VERSION: 0x%02X", *version);

    return ESP_OK;
}
