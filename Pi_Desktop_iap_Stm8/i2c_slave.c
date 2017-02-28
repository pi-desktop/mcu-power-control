

/* Includes ------------------------------------------------------------------*/
#include "hal.h"
#include "led.h"
#include "timer_framework.h"
#include "key.h"
#include "timer.h"
#include "power_ctrl.h"
#include "i2c_slave.h"
#include "stm8s_i2c.h"
#include "stm8s_flash.h"


#pragma section {power_data}

uint8_t Flash_buf[128] = {0};
static  uint8_t Slave_Rx[3] = {0};
uint8_t Tx_Idx = 0, Rx_Idx = 0;
static  uint16_t Event = 0x00;
uint8_t  I2C_IAP_status = I2C_IAP_NULL;
uint16_t Flash_addr = 0;
uint8_t  Flash_len = 0;

#pragma section()


#pragma section (power_text)


void pid_i2c_init(void)
{
	GPIO_DeInit(GPIOB);
	GPIO_Init(GPIOB, (GPIO_Pin_TypeDef)GPIO_PIN_4, GPIO_MODE_OUT_OD_HIZ_SLOW);
	GPIO_Init(GPIOB, (GPIO_Pin_TypeDef)GPIO_PIN_5, GPIO_MODE_OUT_OD_HIZ_SLOW);
	I2C_DeInit();
	I2C_SoftwareResetCmd(DISABLE);
	//I2C_Cmd(ENABLE);
	I2C_Init(100, I2C_IAP_ADDRESS, I2C_DUTYCYCLE_2, I2C_ACK_CURR, I2C_ADDMODE_7BIT, 16);
	I2C_ITConfig((I2C_IT_TypeDef)(I2C_IT_ERR | I2C_IT_EVT | I2C_IT_BUF), ENABLE);
	enableInterrupts();
	I2C_IAP_status = I2C_IAP_NULL;
	Flash_addr = 0x0000;

}

void i2c_slave_intTask(void)
{
	/* Read SR2 register to get I2C error */
	if ((I2C->SR2) != 0)
	{
		/* Clears SR2 register */
		I2C->SR2 = 0;

	}
	//led_sel_type(LED_STYLE_BLINK2);
	
	Event= I2C_GetLastEvent();
	
	switch (Event)
	{
		/******* Slave transmitter ******/
		/* check on EV1 */
		case I2C_EVENT_SLAVE_TRANSMITTER_ADDRESS_MATCHED:
			//Tx_Idx = 0;
			break;

		/* check on EV3 */
		case I2C_EVENT_SLAVE_BYTE_TRANSMITTING:
		{
			/* Transmit data */
			switch (I2C_IAP_status)
			{
				case I2C_IAP_NULL:
				case I2C_IAP_WRITE_REQ:
				case I2C_IAP_WRITE_START:
				{
					I2C_SendData(I2C_IAP_status);
					break;
				}
				case I2C_IAP_READ_ID_REQ:
				{
					I2C_SendData(I2C_IAP_DEVICE_ID);
					I2C_IAP_status = I2C_IAP_NULL;
					break;
				}

				case I2C_IAP_READ_REV_REQ:
				{
					I2C_SendData(FLASH_ReadByte(I2C_IAP_REV_ADDR));
					I2C_IAP_status = I2C_IAP_NULL;
					break;
				}

				case I2C_IAP_READ_FLAG_REQ:
				{
					I2C_SendData(FLASH_ReadByte(I2C_IAP_FLAG_ADDR));
					I2C_IAP_status = I2C_IAP_NULL;
					break;
				}
					
				case I2C_IAP_WRITE_ING:
				{
					I2C_SendData(I2C_IAP_status);
					//I2C_IAP_status = I2C_IAP_NULL;
					//Rx_Idx = 0;
					break;
				}
				case I2C_IAP_READ_START:
				case I2C_IAP_READ_ING:
				{
					I2C_SendData(I2C_IAP_status);
					I2C_IAP_status = I2C_IAP_READ_ING;
					//Tx_Idx = 0;
					break;
				}
				case I2C_IAP_READ_END:
				{
					I2C_SendData(Flash_buf[Tx_Idx]);
					Tx_Idx++;
					if(Tx_Idx == Flash_len)
						I2C_IAP_status = I2C_IAP_NULL;
					break;
				}
				default:
				{
					I2C_SendData(I2C_IAP_status);
					break;
				}
					
				
			}
			//break;
			//I2C_SendData(Slave_Buffer_Rx[Tx_Idx++]);
			break;
		}
		/******* Slave receiver **********/
		/* check on EV1*/
		case I2C_EVENT_SLAVE_RECEIVER_ADDRESS_MATCHED:
			break;

		/* Check on EV2*/
		case I2C_EVENT_SLAVE_BYTE_RECEIVED:
		case 0x0050:
		{
			switch (I2C_IAP_status)
			{
				case I2C_IAP_NULL:
				case I2C_IAP_WRITE_END:
				case I2C_IAP_UPDATE_END:
				{
					Slave_Rx[0] = I2C_ReceiveData();
					I2C_IAP_status = Slave_Rx[0] & 0x0F;
					Tx_Idx = 0;
					Rx_Idx = 0;
					break;
				}
				case I2C_IAP_READ_ID_REQ:
				case I2C_IAP_READ_REV_REQ:
				case I2C_IAP_READ_FLAG_REQ:
				{
					Slave_Rx[0] = I2C_ReceiveData();
					break;
				}
				case I2C_IAP_READ_REQ:
				{
					Slave_Rx[Tx_Idx++] = I2C_ReceiveData();
					if(Tx_Idx == 3)
					{
						Flash_addr = Slave_Rx[0] | (Slave_Rx[1] << 8);
						Flash_len = Slave_Rx[2];
						I2C_IAP_status = I2C_IAP_READ_START;
						Tx_Idx = 0;
					}
					break;
				}
				case I2C_IAP_WRITE_REQ:
				{
					Flash_buf[Rx_Idx++] = I2C_ReceiveData();
					if(Rx_Idx == 3)
					{
						Flash_addr = Flash_buf[0] | (Flash_buf[1] << 8);
						Flash_len = Flash_buf[2];
						I2C_IAP_status = I2C_IAP_WRITE_START;
						Rx_Idx = 0;
					}
					break;
				}
				case I2C_IAP_UPDATE_REQ:
				{
					Flash_buf[Rx_Idx++] = I2C_ReceiveData();
					if(Rx_Idx == 3)
					{
						Flash_addr = Flash_buf[0] | (Flash_buf[1] << 8);
						Flash_len = Flash_buf[2];
						I2C_IAP_status = I2C_IAP_UPDATE_START;
						Rx_Idx = 0;
					}
					break;
				}
					
				case I2C_IAP_RESET_MCU_REQ:
				{
					Flash_buf[Rx_Idx++] = I2C_ReceiveData();
					if(Rx_Idx == 3)
					{
						Flash_addr = Flash_buf[0] | (Flash_buf[1] << 8);
						Flash_len = Flash_buf[2];
						I2C_IAP_status = I2C_IAP_RESET_MCU_START;
						Rx_Idx = 0;
					}
					break;
				}
				case I2C_IAP_WRITE_START:
				{
					Flash_buf[Rx_Idx++] = I2C_ReceiveData();
					if(Rx_Idx == Flash_len)
						I2C_IAP_status = I2C_IAP_WRITE_ING;
					break;
				}
				case I2C_IAP_WRITE_ING:
				{
					Slave_Rx[0] = I2C_ReceiveData();
					break;
				}
				default:
				{
					Slave_Rx[0] = I2C_ReceiveData();
					break;
				}
				
			}
			I2C->SR1;
			I2C->CR2 |= I2C_CR2_ACK;
			break;
		}

		/* Check on EV4 */
		case (I2C_EVENT_SLAVE_STOP_DETECTED):
		{
			/* write to CR2 to clear STOPF flag */
			I2C->SR1;
			I2C->CR2 |= I2C_CR2_ACK;
			break;
		}

		default:
			break;
	}

}
#pragma section()


