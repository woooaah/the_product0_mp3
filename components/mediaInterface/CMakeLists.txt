idf_component_register(
    SRCS "BluetoothClassic/btClassic.c"
    INCLUDE_DIRS "." "BluetoothClassic"
    REQUIRES nvs_flash bt
    )
