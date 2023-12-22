#include "stdio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_err.h"
#include "string.h"
#include "audioStream.h"
#include <inttypes.h>
static audio_streamInfo_t* aud_stream = NULL;
esp_err_t audio_init(void)
{
    return ESP_OK;
}
esp_err_t audio_mp3Reload(audio_streamInfo_t* stream,const char* fileName)
{
    return ESP_OK;
}
int audio_mp3StreamOut(audio_streamInfo_t* stream,uint8_t* out,uint32_t len)
{
    return 0;
}
void audio_mp3Loop(audio_streamInfo_t* stream)
{
}
esp_err_t audio_reload(const char* fileName)
{
    esp_err_t res = 0;
    switch(aud_stream->fileType)
    {
        case audio_FileType_mp3: res = audio_mp3Reload(aud_stream,fileName);break;
        default:break;
    }
    return res;
}
int audio_streamOut(uint8_t* out,uint32_t len)
{
    int res = 0;
    switch(aud_stream->fileType)
    {
        case audio_FileType_mp3: res = audio_mp3StreamOut(aud_stream,out,len);break;
        default:break;
    }
    return res;
}
void audio_loop(void)
{
    switch(aud_stream->fileType)
    {
        case audio_FileType_mp3:audio_mp3Loop(aud_stream);break;
        default:break;
    }
}