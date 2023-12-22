#include "sdmmc_cmd.h"
#include "esp_vfs.h"
#include "esp_vfs_fat.h"

#include <stdlib.h>
#include <string.h>
#include "esp_log.h"

#include "driver/sdmmc_defs.h"
#include "driver/sdmmc_host.h"
#define MOUNT_POINT "/sdcard"

/* interface: sdmmc   system: fatfs (using vfs */
esp_err_t defaut_sdcard_mountfatFs(void)
{

    const char* mount_point = MOUNT_POINT;
    sdmmc_card_t *card;
    // By default, SD card frequency is initialized to SDMMC_FREQ_DEFAULT (20MHz)
    // For setting a specific frequency, use host.max_freq_khz (range 400kHz - 40MHz for SDMMC)
    // Example: for fixed frequency of 10MHz, use host.max_freq_khz = 10000;
    sdmmc_host_t host = SDMMC_HOST_DEFAULT(); /* slot 1 for esp32 */
    // This initializes the slot without card detect (CD) and write protect (WP) signals.
    // Modify slot_config.gpio_cd and slot_config.gpio_wp if your board has these signals.
    sdmmc_slot_config_t slot_config = SDMMC_SLOT_CONFIG_DEFAULT();
    slot_config.width = 1;
    slot_config.flags |= SDMMC_SLOT_FLAG_INTERNAL_PULLUP;

    // sdmmc_read_sectors(card,void);
    esp_vfs_fat_sdmmc_mount_config_t mount_config = 
    {
        .format_if_mount_failed = false,
        .max_files = 5,
        .allocation_unit_size = 0   //4 * 1024 /* 4k unit for fat32*/  TODO: get allocation_unit_size from sdcard fat32 DBR(Disk Boot Record`)
    };
    return esp_vfs_fat_sdmmc_mount(mount_point, &host, &slot_config, &mount_config, &card);
}