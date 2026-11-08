#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__

#include <stdint.h>

/*
 * Frame layout (all multi-byte fields little-endian):
 *
 *   AA | addr | cmd | len | data[len] | crc_lo | crc_hi
 *
 * CRC16/Modbus (poly 0xA001, init 0xFFFF), calculated over addr..data,
 * transmitted low byte first.
 *
 * Response frame uses the same layout with cmd = request cmd | 0x80,
 * and data[0] is the status code.
 */

//frame length:11
#define PROTO_HEAD              0xAA     //帧头
#define PROTO_ADDR              0x01    /* device address */

/* Command codes */
#define WAIT_UPGRADE            0x01
#define BEGIN_TRANSMIT_FIRM     0x03    /* begin firmware transmission */
#define READ_FIRM               0x04    /* read firmware */
#define END_TRANSMIT_FIRM       0x05    /* end firmware transmission */
#define GET_FIRM_VERSION        0x06    /* get firmware version */
#define CONVERT_TO_APP          0x07    /* switch to application mode */
#define PROTO_ACK_FLAG          0x80    /* response cmd = request cmd | 0x80 */
#define COMMAND_SEND_ACK        0x81    /* send acknowledgment */

/* Status codes carried in response data[0] */
typedef enum {
    PROTO_OK                = 0x00,
    PROTO_ERR_FRAME         = 0x01,     /* bad length/CRC/format */
    PROTO_ERR_CMD           = 0x02,     /* unknown command */
    PROTO_ERR_PARAM         = 0x03,     /* bad parameter/length/alignment */
    PROTO_ERR_FLASH_ERASE   = 0x04,
    PROTO_ERR_FLASH_WRITE   = 0x05,
    PROTO_ERR_STATE         = 0x06,     /* wrong state / offset mismatch */
    PROTO_ERR_VERIFY        = 0x07,     /* size or CRC mismatch */
    PROTO_ERR_RANGE         = 0x08      /* address/size out of range */
} proto_status_t;

/* Business callbacks, implemented by the application (bootloader / app) */
uint16_t CalcCRC(uint32_t addr, uint32_t len);
void Protocol_Handler(uint8_t *frame, uint16_t len);
#endif /* __PROTOCOL_H__ */
