idf_component_register(
    SRCS
        "main.c"
        "board_gpio.c"
        "board_adc.c"
        "cc1101.c"
        "cc1101_config.c"
    INCLUDE_DIRS "."
    REQUIRES
        esp_driver_gpio
        esp_driver_spi
        esp_adc
)