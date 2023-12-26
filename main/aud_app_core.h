#ifndef __AUD_APP_CORE_H__
#define __AUD_APP_CORE_H__

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "mp3d.h"
/* log tag */
#define AUD_APP_CORE_TAG             "AUD_APP_CORE"
typedef struct {
    mp3AudioStream_t* stream;
} aud_app_msg_t;
bool aud_app_send_msg(aud_app_msg_t* msg);
void aud_app_task_start_up(void);
void aud_app_task_shut_down(void);
void aud_loop(void);

#endif