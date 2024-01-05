//this file NOT a part of LittleFS lib
//this file consist only some prototypes of function and configuration data

#ifdef __cplusplus
 extern "C" {
#endif


#include "lfs.h"

lfs_t lfs;
lfs_file_t file;
struct lfs_config cfg;

int io_fs_init(uint8_t * lfsReadBuff, uint8_t * lfsWriteBuff, uint8_t * lfslookaheadBuff, uint16_t buffers_size, uint32_t flash_r_size, uint32_t flash_w_size, uint32_t flash_erse_size, uint32_t block_count);

#ifdef __cplusplus
} /* extern "C" */
#endif
