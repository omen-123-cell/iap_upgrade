#ifndef __MEMORY_H__
#define __MEMORY_H__

#include "stm32f4xx_hal.h"
#define UPGRADE_MAGIC     0xAA55AA55   

typedef struct {
    uint32_t magic;
    uint32_t app_size;
    uint16_t app_crc;
    uint16_t app_version;      /* firmware version from BEGIN_TRANSMIT_FIRM */
} upgrade_info_t;


#define STORAGE_ADDR        0x08060000   
#define UPGRADE_INFO_ADDR   STORAGE_ADDR  
#define APP_DATA_ADDR       (STORAGE_ADDR + 512)  

#endif
