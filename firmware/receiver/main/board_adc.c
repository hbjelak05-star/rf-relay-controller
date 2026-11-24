
#include <math.h>

#include "board_adc.h"
#include "board_pins.h"

#include "esp_adc/adc_oneshot.h"
#include "esp_adc/adc_cali.h"
#include "esp_adc/adc_cali_scheme.h"

// NTC parameters
#define NTC_FIXED_RESISTOR  10000.0f
#define NTC_R0              10000.0f
#define NTC_T0              298.15f
#define NTC_BETA            3380.0f

// ADC configuration
#define NTC_ADC_UNIT        ADC_UNIT_1
#define NTC_ADC_CHANNEL     ADC_CHANNEL_0
#define NTC_ADC_ATTEN       ADC_ATTEN_DB_12

static adc_oneshot_unit_handle_t adc_handle;
static adc_cali_handle_t cali_handle;

esp_err_t board_adc_init(void)
{
    adc_oneshot_unit_init_cfg_t unit_config = {
        .unit_id = NTC_ADC_UNIT,
    };

    esp_err_t ret = adc_oneshot_new_unit(
        &unit_config, &adc_handle);

    if (ret != ESP_OK) {
        return ret;
    }

    adc_oneshot_chan_cfg_t channel_config = {
        .bitwidth = ADC_BITWIDTH_DEFAULT,
        .atten = NTC_ADC_ATTEN,
    };

    ret = adc_oneshot_config_channel(
        adc_handle,
        NTC_ADC_CHANNEL,
        &channel_config);

    if (ret != ESP_OK) {
        return ret;
    }

    adc_cali_line_fitting_config_t cali_config = {
        .unit_id = NTC_ADC_UNIT,
        .atten = NTC_ADC_ATTEN,
        .bitwidth = ADC_BITWIDTH_DEFAULT,
    };

    ret = adc_cali_create_scheme_line_fitting(
        &cali_config, &cali_handle);

    return ret;
}

esp_err_t ntc_read_temperature(float *temperature_c)
{
    if (temperature_c == NULL) {
        return ESP_ERR_INVALID_ARG;
    }

    int raw_value;
    int voltage_mv;

    esp_err_t ret = adc_oneshot_read(
        adc_handle,
        NTC_ADC_CHANNEL,
        &raw_value);

    if (ret != ESP_OK) {
        return ret;
    }

    ret = adc_cali_raw_to_voltage(
        cali_handle,
        raw_value,
        &voltage_mv);

    if (ret != ESP_OK) {
        return ret;
    }

    float voltage = voltage_mv / 1000.0f;

    if (voltage <= 0.0f || voltage >= 3.3f) {
        return ESP_ERR_INVALID_RESPONSE;
    }

    float resistance =
        NTC_FIXED_RESISTOR * voltage / (3.3f - voltage);

    
    float temperature_kelvin =
        1.0f / (1.0f / NTC_T0 +
        logf(resistance / NTC_R0) / NTC_BETA);

    *temperature_c = temperature_kelvin - 273.15f;

    return ESP_OK;
}
