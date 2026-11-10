#ifndef __FLASH_IF_H__
#define __FLASH_IF_H__

#include "stm32f4xx_hal.h"

#define FLASH_BASE_ADDR       0x08000000
#define FLASH_TOTAL_SIZE      (512 * 1024)

//Boot square
#define ADDR_BOOT             0x08000000
#define SIZE_BOOT             (64 * 1024)   // 64KB

//Run square
#define ADDR_RUN             0x08010000
#define SIZE_RUN             (256 * 1024)    // 256KB 

//Memory square
#define ADDR_MEMORY             0x08060000
#define SIZE_MEMORY             (128 * 1024)  // 128KB

typedef void (*pFunction)(void);
uint32_t FLASH_If_GetSector(uint32_t addr);
int FLASH_If_EraseSector(uint32_t sector);
int FLASH_Write_Buffer(uint32_t start_addr, uint8_t *buf, uint32_t len);
void FLASH_If_Read(uint32_t addr, uint8_t *buf, uint32_t len);
uint16_t FLASH_If_CalcCRC(uint32_t addr, uint32_t len);
void Jump_To_RunArea(uint32_t app_addr);
int Copy_Firmware(void);

#endif
