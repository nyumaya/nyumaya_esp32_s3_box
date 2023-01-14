
idf_component_register(
    SRCS main.cc main_functions.cc models/marvin_v3.1.286.cc ringbuf.c
    PRIV_REQUIRES spi_flash bsp libnyumaya_esp32
    INCLUDE_DIRS "models")


