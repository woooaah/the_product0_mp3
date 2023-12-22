idf_component_register(
    SRCS "sdcard.c" "fileSystem.c"
    INCLUDE_DIRS "."
    REQUIRES sdmmc fatfs vfs)
