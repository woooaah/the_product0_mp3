#include "fileSystem.h"
#include "dirent.h"
#include "esp_log.h"
#include "string.h"
static const char *TAG = "fileSystem";


static DIR *dp = NULL;
static struct dirent fileListBuffer[10];
static int numOfList = 0;
esp_err_t fileSystem_init(void)
{
    esp_err_t err = defaut_sdcard_mountfatFs();
    if (err != ESP_OK) {
        if (err == ESP_FAIL) {
            ESP_LOGE(TAG, "Failed to mount filesystem. "
                     "If you want the card to be formatted, set the EXAMPLE_FORMAT_IF_MOUNT_FAILED menuconfig option.");
        } else {
            ESP_LOGE(TAG, "Failed to initialize the card (%s). "
                     "Make sure SD card lines have pull-up resistors in place.", esp_err_to_name(err));
        }
        return ESP_FAIL;
    }
    ESP_LOGI(TAG, "mounted");

    return fileSystem_enterRoot();
}

esp_err_t fileSystem_enterRoot(void)
{
    return fileSystem_changeDir("/sdcard");
}
esp_err_t fileSystem_listLoad(int target_num)
{
    struct dirent* curDirEntry;
    int num = 0;
    while((curDirEntry = readdir(dp))!=NULL && num<target_num)
    {
        memcpy(&fileListBuffer[numOfList],curDirEntry,sizeof(struct dirent));
        num++;
        numOfList++;
    }
    return ESP_OK; 
}
esp_err_t fileSystem_changeDir(const char* dirName)
{
    if((dp = opendir(dirName)) == NULL)
    {
        printf("error, no such dir %s\n ",dirName);
        return ESP_ERR_NOT_FOUND;
    }
    return ESP_OK;
    
}
esp_err_t fileSystem_listShow(void)
{
    
    for(int i=0;i<numOfList;i++)
    {
        printf("ind:%d   ;",fileListBuffer[i].d_ino);
        switch(fileListBuffer[i].d_type)
        {
            case DT_DIR:printf("DIRECTORY:");break;
            case DT_REG:printf("FILE     :");break;
            default:    printf("NONE     :");break;
        }
        printf("%s\n",fileListBuffer[i].d_name);
    };
    return ESP_OK;
}
    