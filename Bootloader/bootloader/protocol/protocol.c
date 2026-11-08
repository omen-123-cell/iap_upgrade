#include "protocol.h"
#include "usart.h"
#include "flash_if.h"

static uint16_t firm_size;
uint16_t total_size;
uint16_t target_crc;
uint16_t total_crc_cal;


uint16_t CalcCRC(uint32_t addr, uint32_t len)
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


void Protocol_Handler(uint8_t *frame, uint16_t len)
{

	if(frame[0] != PROTO_HEAD)
		return;
	if(frame[1] != PROTO_ADDR)
		return;

	//crc calculate frame
	 uint16_t cal_crc = CalcCRC((uint32_t)frame, len - 2);
   uint16_t rec_crc = frame[len - 2] | (frame[len - 1] << 8);
   if (cal_crc != rec_crc) {
       Send_Response(frame[2] , PROTO_ERR_FRAME, NULL , 0 ); // verify fault
       return;
   }
	 
	 //verify data
	 uint8_t cmd = frame[2];
	 uint8_t length = frame[3];
	 int err;
	 
	 switch(cmd)
	 {

		 case WAIT_UPGRADE:
		 {
			//初始化，等待开始传输指令
			Send_Response(frame[2] , PROTO_OK, NULL, 0);
		 }	
		 break;
		 
		 //接收固件的总长度和目标crc
		 case BEGIN_TRANSMIT_FIRM:
		 {
			 total_size = (uint16_t)frame[3] | ((uint16_t)frame[4] << 8); 
			 target_crc = (uint16_t)frame[5] | ((uint16_t)frame[6] << 8);
			 err = FLASH_If_EraseSector(FLASH_SECTOR_4);
			 firm_size = 0;
			 if(err == 0)
				Send_Response(frame[2] , PROTO_OK, NULL, 0);
		 }
		 break;
		 
		 case READ_FIRM:
		 {
			uint32_t write_addr = ADDR_RUN + firm_size;
			 
			if (FLASH_Write_Buffer(write_addr, &frame[4], length) == 0) {

        firm_size += length; 
        Send_Response(cmd, PROTO_OK, NULL, 0);
			} else {

        Send_Response(cmd, PROTO_ERR_FLASH_WRITE, NULL, 0);
			}
		}
		 break;
		 
		 case END_TRANSMIT_FIRM:
		 {
			 total_crc_cal = CalcCRC(ADDR_RUN,firm_size);
			 if(total_crc_cal == target_crc)
			 {
				Send_Response(frame[2] , PROTO_OK, NULL, 0);
				HAL_Delay(100);
				Jump_To_RunArea(ADDR_RUN);
			 }
			 else
			 {
				Send_Response(frame[2] , PROTO_ERR_VERIFY, NULL, 0);
			 }
			 
		 }
		 break;
        
		 case GET_FIRM_VERSION:
		 {
        uint8_t ver[3] = {0x0A, 0x0B, 0x0C}; 
        Send_Response(cmd, PROTO_OK, ver, 3); 
		}
     break;
		
     default:
		 {
        Send_Response(cmd, PROTO_ERR_CMD, NULL, 0);
       
		 }
        break;
	 }		 
}

