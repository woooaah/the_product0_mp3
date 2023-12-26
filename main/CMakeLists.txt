idf_component_register(SRCS "app_main.c" "bt_app_core.c" "aud_app_core.c"
                    INCLUDE_DIRS ""
                    REQUIRES AppStateModel storage mediaInterface decoder
                    PRIV_REQUIRES spi_flash)