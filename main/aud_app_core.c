#include "aud_app_core.h"
#include "freertos/FreeRTOSConfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "mp3d.h"

/*********************************
 * STATIC VARIABLE DEFINITIONS
 ********************************/
static QueueHandle_t s_aud_app_task_queue = NULL;
static TaskHandle_t s_aud_app_task_handle = NULL;
static void aud_app_work_dispatched(aud_app_msg_t* msg)
{
    if(msg->stream)
    {
        audio_a2dpSourceLoadTask(msg->stream);
    }
}
static void aud_app_task_handler(void *arg)
{
    aud_app_msg_t msg;

    for (;;) {
        /* receive message from work queue and handle it */
        if (pdTRUE == xQueueReceive(s_aud_app_task_queue, &msg, (TickType_t)portMAX_DELAY)) {
            // ESP_LOGD(AUD_APP_CORE_TAG, "%s, signal: 0x%x, event: 0x%x", __func__, msg.sig, msg.event);
            aud_app_work_dispatched(&msg);
        }
    }
}
void aud_app_task_start_up(void)
{
    s_aud_app_task_queue = xQueueCreate(10, sizeof(aud_app_msg_t));
    // xTaskCreate(aud_app_task_handler, "AudAppTask", 8192, NULL, 18, &s_aud_app_task_handle);
}
bool aud_app_send_msg(aud_app_msg_t* msg)
{
    if (msg == NULL) {
        return false;
    }

    if (pdTRUE != xQueueSend(s_aud_app_task_queue, msg, 10 / portTICK_PERIOD_MS)) {
        ESP_LOGE(AUD_APP_CORE_TAG, "%s xQueue send failed", __func__);
        return false;
    }

    return true;
}
void aud_app_task_shut_down(void)
{
    if (s_aud_app_task_handle) {
        vTaskDelete(s_aud_app_task_handle);
        s_aud_app_task_handle = NULL;
    }
    if (s_aud_app_task_queue) {
        vQueueDelete(s_aud_app_task_queue);
        s_aud_app_task_queue = NULL;
    }
}

void aud_loop(void)
{
    aud_app_task_handler(NULL);
}