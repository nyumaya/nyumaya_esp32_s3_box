
idf_component_register(
    SRCS main.cc main_functions.cc marvin_model.cc ringbuf.c
    PRIV_REQUIRES spi_flash bsp libnyumaya_esp32
    INCLUDE_DIRS "")


