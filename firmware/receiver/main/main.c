#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_log.h"
#include "cc1101.h"

static const char *TAG = "RECEIVER";

void app_main(void)
{
    uint8_t part_number;
    uint8_t version;

    ESP_LOGI(TAG, "Starting receiver...");

    if (cc1101_init() != ESP_OK) {
        ESP_LOGE(TAG, "CC1101 initialization failed");
        return;
    }

    if (cc1101_read_id(&part_number, &version) != ESP_OK) {
        ESP_LOGE(TAG, "Could not read CC1101 identification");
        return;
    }

    ESP_LOGI(TAG, "CC1101 communication successful!");

    while (1) {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}