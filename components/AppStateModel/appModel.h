
#ifndef APPMODEL_H_
#define APPMODEL_H_
#include "audioStream.h"

void hardware_init(void);

/* sd card mount */
void hardware_storageInit(void);
/* lvgl init */
void hardware_screenInit(void);
/* i2s and bluetooth init */
void hardware_audioInit(void);

typedef enum
{
    audioIf_i2s,
    audioIf_bt_classic,
    audioIf_bt_ble
}audioInterface_t;
/* i2s */
void hardware_selectAudioInterface(audioInterface_t audioIf);



#endif