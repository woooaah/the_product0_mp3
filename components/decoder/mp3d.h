#ifndef MP3D_H_
#define MP3D_H_
#include "stdint.h"
#include "stdio.h"
#include "esp_a2dp_api.h"
#include "minimp3.h"
typedef struct 
{
    FILE*    fp;
    uint8_t* datas;    
    uint32_t pOut;
    uint32_t pIn;
    uint32_t pLast;
    uint32_t datasSize;
    uint32_t maxReadBytes;
}mp3FileStream_t;

mp3FileStream_t* createMp3FileStream(const char* fileName,uint32_t datasSize,uint32_t minKeepSize,uint32_t maxReadBytes);
void reloadMp3FileStream(mp3FileStream_t* stream);



typedef struct 
{
    int16_t* datas;
    uint32_t pOut;
    uint32_t pIn;
    uint32_t pLast;
    uint32_t datasSize;
}mp3PcmStream_t;

mp3PcmStream_t* createMp3PcmStream(uint32_t datasSize,uint32_t minKeepSize);
int32_t mp3dToA2dpDatacb(mp3PcmStream_t* stream, uint8_t *buf, int32_t len);

typedef struct
{
    mp3FileStream_t*        fs;
    mp3PcmStream_t*         ps;
    mp3dec_frame_info_t*    info;
    mp3dec_t*               mp3d;
}mp3AudioStream_t;

mp3AudioStream_t* createMp3audioStream(const char* fileName);


void audio_prepare(mp3AudioStream_t* stream);
void audio_a2dpSourceLoadTask(mp3AudioStream_t* stream);
#endif