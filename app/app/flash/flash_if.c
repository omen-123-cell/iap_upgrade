#include "stm32f4xx_hal.h"
#include "flash_if.h"


//根据地址指定扇区
uint32_t FLASH_If_GetSector(uint32_t addr)
{
    uint32_t sector = 0;

    if (addr < 0x08004000)
        sector = FLASH_SECTOR_0;
    else if (addr < 0x08008000)
        sector = FLASH_SECTOR_1;
    else if (addr < 0x0800C000)
        sector = FLASH_SECTOR_2;
    else if (addr < 0x08010000)
        sector = FLASH_SECTOR_3;
    else if (addr < 0x08020000)
        sector = FLASH_SECTOR_4;
    else if (addr < 0x08040000)
        sector = FLASH_SECTOR_5;
    else if (addr < 0x08060000)
        sector = FLASH_SECTOR_6;
    else
        sector = FLASH_SECTOR_7;

    return sector;
}

// 删除扇区
int FLASH_If_EraseSector(uint32_t sector)
{
    FLASH_EraseInitTypeDef EraseInitStruct;
    uint32_t SectorError = 0;
    int ret = 0;

    HAL_FLASH_Unlock();

    EraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS;
    EraseInitStruct.Sector = sector;
    EraseInitStruct.NbSectors = 1;
    EraseInitStruct.VoltageRange = FLASH_VOLTAGE_RANGE_3;

    if (HAL_FLASHEx_Erase(&EraseInitStruct, &SectorError) != HAL_OK) {
        ret = -1;  
    }

    HAL_FLASH_Lock();
    return ret;
}

// FLASH编程字
int FLASH_If_Write(uint32_t addr, uint32_t *data, uint32_t len)
{
    int ret = 0;

    if (addr % 4 != 0) {
        return -1;
    }

    HAL_FLASH_Unlock();

    for (uint32_t i = 0; i < len; i++) {
        if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD,
                              addr + i * 4,
                              data[i]) != HAL_OK) {
            ret = -1;
            break;
        }
    }

    HAL_FLASH_Lock();
    return ret;
}

// 读取FLASH
void FLASH_If_Read(uint32_t addr, uint8_t *buf, uint32_t len)
{
    for (uint32_t i = 0; i < len; i++) {
        buf[i] = *(uint8_t*)(addr + i);
    }
}

// 
uint16_t FLASH_If_CalcCRC(uint32_t addr, uint32_t len)
{
    uint16_t crc = 0xFFFF;
    for (uint32_t i = 0; i < len; i++) {
        uint8_t byte = *(uint8_t*)(addr + i);
        crc ^= byte;
        for (uint8_t j = 0; j < 8; j++) {
            if (crc & 0x0001)
                crc = (crc >> 1) ^ 0xA001;
            else
                crc >>= 1;
        }
    }
    return crc;
}

