#ifndef AUDIOSTREAM_H_
#define AUDIOSTREAM_H_
#include "stdio.h"
#include "esp_err.h"



#define DECODER_FILE_BUFFER_SIZE (32*1024U)
#define DECODER_PCM_BUFFER_SIZE  (MINIMP3_MAX_SAMPLES_PER_FRAME*10U)

typedef enum
{
    audio_FileType_mp3,
    audio_FileType_flac,
    audio_FileType_wav,
    audio_FileType_other
}audio_FileType_t;


typedef struct 
{
    FILE *filePtr;
    audio_FileType_t fileType;
    uint8_t* fileBuf;
    int fl;
    int fr;
    int16_t* pcmBuf;
    int pl;
    int pr;
    int ps;
    /* mp3 decoder */
    int mp3Samples;
    /* ... */
} audio_streamInfo_t;

int audio_streamOut(uint8_t* out,uint32_t len);

esp_err_t audio_reload(const char* fileName);

void audio_outFile(const char* fileName);
void audio_loop(void);
esp_err_t audio_init(void);
#endif