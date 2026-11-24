idf_component_register(
    SRCS
        "main.c"
        "board_gpio.c"
        "board_adc.c"
    INCLUDE_DIRS "."
    REQUIRES esp_driver_gpio esp_adc
)