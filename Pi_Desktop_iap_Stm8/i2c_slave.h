/*****************************************************************************
 *
 *	Copyright(c) MNDZ, 2008. All rights reserved.
 *
 *	Description:
 *
 *	$Source: $
 *	$Author: $
 *	$Revision:$
 *	$Date: $
 *
 *****************************************************************************/

#ifndef __I2C_SLAVE_H
#define __I2C_SLAVE_H


/****************************************************************************
 * #include section
 *	add #include here if any
 ***************************************************************************/
#include "stm8s.h"
//#include "iostm8.h"

/****************************************************************************
 * #define section
 *	add constant #define here if any
 ***************************************************************************/

#define I2C_IAP_ADDRESS                0x30
#define I2C_IAP_DEVICE_ID              0xE7
#define I2C_IAP_REV_ADDR               0x4055
#define I2C_IAP_FLAG_ADDR              0x4065
#define I2C_IAP_FLAG_VAL               0xA5



#define I2C_IAP_NULL                   0xAB

#define I2C_IAP_READ_ID_REQ            1

#define I2C_IAP_READ_REQ               2
#define I2C_IAP_READ_START             0x12
#define I2C_IAP_READ_ING               0x22
#define I2C_IAP_READ_END               0x32

#define I2C_IAP_WRITE_REQ              3
#define I2C_IAP_WRITE_START            0x13
#define I2C_IAP_WRITE_ING              0x23
#define I2C_IAP_WRITE_END              0x33

#define I2C_IAP_UPDATE_REQ             0x4
#define I2C_IAP_UPDATE_START           0x14
#define I2C_IAP_UPDATE_ING             0x24
#define I2C_IAP_UPDATE_END             0x34

#define I2C_IAP_READ_REV_REQ           5

#define I2C_IAP_READ_FLAG_REQ          6
#define I2C_IAP_FLAG_MAGIC             0xa5a5


#define I2C_IAP_RESET_MCU_REQ          7
#define I2C_IAP_RESET_MCU_START        0x17
#define I2C_IAP_RESET_MAGIC_H          0xaa55
#define I2C_IAP_RESET_MAGIC_L          0xa5



#define I2C_IAP_READ_ID                0xE1
#define I2C_IAP_READ                   0xE2
#define I2C_IAP_WRITE                  0xE3
#define I2C_IAP_UPDATE                 0xE4
#define I2C_IAP_READ_REV               0xE5
#define I2C_IAP_READ_FLAG              0xE6
#define I2C_IAP_RESET_MCU              0xE7
#define I2C_IAP_TEST                   0xEF


#define I2C_IAP_STATUS_BUSY            0x01
#define I2C_IAP_STATUS_TOUT            0x02
#define I2C_IAP_STATUS_REER            0x04
#define I2C_IAP_STATUS_WEER            0x08
#define I2C_IAP_STATUS_UEER            0x10






/****************************************************************************
 * ADT section
 *	add Abstract Data Type definition here
 ***************************************************************************/

/****************************************************************************
 *  extern variable declaration section
 ***************************************************************************/
extern uint8_t  I2C_IAP_status;
extern uint16_t Flash_addr;
extern uint8_t  Flash_len;
extern uint8_t Tx_Idx;
extern uint8_t Rx_Idx;


extern uint8_t Flash_buf[128];



/****************************************************************************
 *  section
 *	add function prototype here if any
 ***************************************************************************/

void pid_i2c_init(void);
void i2c_slave_intTask(void);




#endif

