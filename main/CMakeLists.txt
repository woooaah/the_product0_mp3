idf_component_register(SRCS "app_main.c" "bt_app_core.c"
                    INCLUDE_DIRS ""
                    REQUIRES AppStateModel storage audio mediaInterface
                    PRIV_REQUIRES spi_flash)