#include "stm32f4xx_hal.h"
#include "flash_if.h"
#include "usart.h"


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
    EraseInitStruct.NbSectors = 3;
    EraseInitStruct.VoltageRange = FLASH_VOLTAGE_RANGE_3;

    if (HAL_FLASHEx_Erase(&EraseInitStruct, &SectorError) != HAL_OK) {
        ret = -1;  
    }

    HAL_FLASH_Lock();
    return ret;
}

// FLASH编程字
int FLASH_Write_Buffer(uint32_t start_addr, uint8_t *buf, uint32_t len)
{
    int ret = 0;
    uint32_t write_addr = start_addr;
    uint32_t *data_ptr = (uint32_t *)buf;

    if (start_addr % 4 != 0) return -1;

    HAL_FLASH_Unlock();
    
    for (uint32_t i = 0; i < len / 4; i++) {
        if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, write_addr, data_ptr[i]) != HAL_OK) {
            ret = -1;
            break;
        }
        write_addr += 4; 
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



void Jump_To_RunArea(uint32_t app_addr)
{ 
    pFunction Jump_To_Application;
    uint32_t JumpAddress;

    if (((*(__IO uint32_t*)app_addr) & 0x2FFE0000) == 0x20000000)
    {

        JumpAddress = *(__IO uint32_t*) (app_addr + 4);
        Jump_To_Application = (pFunction) JumpAddress;


        HAL_UART_DeInit(&huart1); 
        HAL_RCC_DeInit();  
        HAL_DeInit();      


        SysTick->CTRL = 0;
        SysTick->LOAD = 0;
        SysTick->VAL = 0;


        for (uint8_t i = 0; i < 8; i++) {
            NVIC->ICER[i] = 0xFFFFFFFF;
            NVIC->ICPR[i] = 0xFFFFFFFF;
        }


        __set_MSP(*(__IO uint32_t*)app_addr);
        

        __disable_irq();
        

        Jump_To_Application();
    }

}

