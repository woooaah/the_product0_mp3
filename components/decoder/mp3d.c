#include "minimp3/minimp3.h"
#include "mp3d.h"
#include "esp_err.h"
#include "string.h"
mp3FileStream_t* createMp3FileStream(const char* fileName,uint32_t datasSize,uint32_t minKeepSize,uint32_t maxReadBytes)
{
    if(2*minKeepSize > datasSize)
    {
        printf("UserConfigError, mp3decoderInputBufferSize is less than 2* minium keep size");
        return NULL;
    }
    mp3FileStream_t* stream = (mp3FileStream_t*)calloc(1,sizeof(mp3FileStream_t));
    stream->datas = (uint8_t*)calloc(datasSize,sizeof(uint8_t));
    stream->datasSize = datasSize;
    stream->fp = fopen(fileName,"r");
    stream->pLast = datasSize- minKeepSize;
    fread(stream->datas,1,datasSize,stream->fp);
    stream->pIn = datasSize;
    stream->maxReadBytes = maxReadBytes;
    return stream;
}



mp3PcmStream_t* createMp3PcmStream(uint32_t datasSize,uint32_t minKeepSize)
{
    mp3PcmStream_t* stream = (mp3PcmStream_t*)calloc(1,sizeof(mp3PcmStream_t));
    stream->datas = (int16_t*)calloc(datasSize,sizeof(int16_t));
    stream->datasSize = datasSize;
    stream->pLast = datasSize-minKeepSize;
    return stream;
}


mp3AudioStream_t* createMp3audioStream(const char* fileName)
{
    mp3AudioStream_t* s = (mp3AudioStream_t*)calloc(1,sizeof(mp3AudioStream_t));
    s->fs = createMp3FileStream(fileName,32*1024,16*1024,16*1024);
    s->ps = createMp3PcmStream(MINIMP3_MAX_SAMPLES_PER_FRAME*2,MINIMP3_MAX_SAMPLES_PER_FRAME);
    s->mp3d = (mp3dec_t*)calloc(1,sizeof(mp3dec_t));
    s->info = (mp3dec_frame_info_t*)calloc(1,sizeof(mp3dec_frame_info_t));
    mp3dec_init(s->mp3d);
    return s;
}

/* main three func is below */


int32_t mp3dToA2dpDatacb(mp3PcmStream_t* stream, uint8_t *out, int32_t len)
{
    int size = stream->pIn - stream->pOut;
    size = size<(len/2)?size:(len/2);
    memcpy((int16_t*)out,stream->datas + stream->pOut,size*2);
    stream->pOut += size;
    // printf("output %d bytes to a2d stream!\n",size*2);
    return size*2;
}

void reloadMp3FileStream(mp3FileStream_t* stream)
{
    int readSize,leftSize;
    int resSize;
    if(stream->pOut >= stream->pLast)
    {
        memcpy(stream->datas,stream->datas+stream->pOut,stream->pIn-stream->pOut);
        stream->pIn -= stream->pOut;
        stream->pOut = 0;
        // printf("fileBuf shift occurs;");
    }
    if(stream->pIn<stream->datasSize)
    {
        leftSize = stream->datasSize-stream->pIn;
        readSize = leftSize<stream->maxReadBytes?leftSize:stream->maxReadBytes;
        resSize= fread(stream->datas+stream->pIn,1,readSize,stream->fp);
        stream->pIn += resSize;
        // printf("fread %d bytes\n",resSize);
    }
}
void changeMp3FileStreamMaxReadBytes(mp3FileStream_t* stream,uint32_t maxReadBytes)
{
    stream->maxReadBytes = maxReadBytes;
}
void audio_loadPcm(mp3AudioStream_t* stream)
{
    int samples;
    if(stream->ps->pOut>=stream->ps->pLast)
    {
        memcpy(stream->ps->datas,stream->ps->datas+stream->ps->pOut,stream->ps->pIn-stream->ps->pOut);
        stream->ps->pIn -= stream->ps->pOut;
        stream->ps->pOut = 0;
        // printf("pcmBuf shift occurs;pIn:%lu,pOut:%lu",stream->ps->pIn,stream->ps->pOut);
    }
    if(stream->ps->pIn<=stream->ps->pLast)
    {
        // printf("decode occurs fs pIn%lu, fs pOut%lu, pcm pIn%lu,pcm pOut%lu;",stream->fs->pIn,stream->fs->pOut,stream->ps->pIn,stream->ps->pOut);
        samples = mp3dec_decode_frame(stream->mp3d,stream->fs->datas+stream->fs->pOut,stream->fs->pIn-stream->fs->pOut,stream->ps->datas,stream->info); //+stream->ps->pIn
        stream->fs->pOut += stream->info->frame_bytes;
        stream->ps->pIn += samples*stream->info->channels;
        // printf("pass %d frame_bytes, get %d samples\n",stream->info->frame_bytes,samples);
    }
    else
    {
        // printf("decoder has not enough space!\n");
    }

}

/* 
    prepare data for data_cb 
*/
void audio_prepare(mp3AudioStream_t* stream)
{
    while(stream->ps->pIn<=stream->ps->pLast)
    {
        reloadMp3FileStream(stream->fs);
        audio_loadPcm(stream);
    }
    changeMp3FileStreamMaxReadBytes(stream->fs,1024);
}

void audio_a2dpSourceLoadTask(mp3AudioStream_t* stream)
{
    reloadMp3FileStream(stream->fs);
    audio_loadPcm(stream);
}