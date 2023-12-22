#ifndef FILESYSTEM_H_
#define FILESYSTEM_H_
#include "sdcard.h"

esp_err_t fileSystem_init(void);

esp_err_t fileSystem_listLoad(int target_num);

esp_err_t fileSystem_changeDir(const char* dirName);

esp_err_t fileSystem_listShow(void);

esp_err_t fileSystem_enterRoot(void);
#endif
