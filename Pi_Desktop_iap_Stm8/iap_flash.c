

/* Includes ------------------------------------------------------------------*/
#include "hal.h"
#include "led.h"
#include "timer_framework.h"
#include "key.h"
#include "timer.h"
#include "iap_flash.h"
#include "i2c_slave.h"
#include "stm8s_flash.h"
#include "power_ctrl.h"
#include "stm8s_iwdg.h"




/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/
/* Declare _fctcpy function prototype as it is packaged by default in the Cosmic
   machine library */
#ifdef _COSMIC_
 int _fctcpy(char name);
#endif /*_COSMIC_*/


#pragma section (power_text)

void FLASH_Config(void)
{
	/* Define flash programming Time*/
	FLASH_SetProgrammingTime(FLASH_PROGRAMTIME_STANDARD);

	FLASH_Unlock(FLASH_MEMTYPE_PROG);
	/* Wait until Flash Program area unlocked flag is set*/
	while (FLASH_GetFlagStatus(FLASH_FLAG_PUL) == RESET)
	{}

	/* Unlock flash data eeprom memory */
	FLASH_Unlock(FLASH_MEMTYPE_DATA);
	/* Wait until Data EEPROM area unlocked flag is set*/
	while (FLASH_GetFlagStatus(FLASH_FLAG_DUL) == RESET)
	{}
}

void Delay(uint16_t nCount)
{
    /* Decrement nCount value */
    while (nCount != 0)
    {
        nCount--;
    }
}

static void IWDG_Config(void)
{
  /* Enable IWDG (the LSI oscillator will be enabled by hardware) */
  IWDG_Enable();
  
  /* IWDG timeout equal to 250 ms (the timeout may varies due to LSI frequency
     dispersion) */
  /* Enable write access to IWDG_PR and IWDG_RLR registers */
  IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
  
  /* IWDG counter clock: LSI/128 */
  IWDG_SetPrescaler(IWDG_Prescaler_128);
  
  /* Set counter reload value to obtain 250ms IWDG Timeout.
    Counter Reload Value = 250ms/IWDG counter clock period
                         = 250ms / (LSI/128)
                         = 0.25s / (LsiFreq/128)
                         = LsiFreq/(128 * 4)
                         = LsiFreq/512
   */
  IWDG_SetReload((uint8_t)(2));
  
  /* Reload IWDG counter */
  //IWDG_ReloadCounter();
}


void IAP_Flash_test(void)
{
	int i;
	for (i=0;i<128;i++)
	{
		FLASH_ProgramByte(0x4000+i, 0x00);
	}

}

void IAP_Flash_init(void)
{
		
#ifdef _COSMIC_
	/* Call the _fctcpy() function with the first segment character as parameter 
	"_fctcpy('F');"  for a manual copy of the declared moveable code segment
	(FLASH_CODE) in RAM before execution*/
	_fctcpy('F');
#endif /*_COSMIC_*/
	

	/* FLASH Configuration ------------------------------------------*/
	FLASH_Config();

}




void IAP_Flash_read(void)
{

}
void IAP_Flash_write(void)
{

}


void IAP_Flash_update(void)
{
	uint16_t addr = 0;
	uint8_t val = 0;

	val = FLASH_ReadByte(I2C_IAP_REV_ADDR);

	if((Flash_addr == I2C_IAP_FLAG_MAGIC) && (val < Flash_len))
	{
		FLASH_ProgramByte(I2C_IAP_FLAG_ADDR, I2C_IAP_FLAG_VAL);
		FLASH_ProgramByte(I2C_IAP_REV_ADDR, Flash_len);
	}
}

void Reset_mcu(void)
{
	if((I2C_IAP_RESET_MAGIC_H == Flash_addr) && (Flash_len == I2C_IAP_RESET_MAGIC_L))
	{
		IWDG_Config();
	}
}


void IAP_Flash_main(void)
{
	uint16_t add = 0;
	uint16_t BlockNum;


	period_tasks();
	power_ctrl();


	switch (I2C_IAP_status)
	{
		
		case I2C_IAP_READ_START:
		case I2C_IAP_READ_ING:
		{
			Tx_Idx = 0;
			for (add = Flash_addr; add < Flash_addr + Flash_len; add++)
			{
				Flash_buf[Tx_Idx++] = FLASH_ReadByte(add);
			}
			I2C_IAP_status = I2C_IAP_READ_END;
			Tx_Idx = 0;
			break;
		}
		case I2C_IAP_WRITE_ING:
		{
			Rx_Idx = 0;
			BlockNum = (Flash_addr -0x8000)/128;
			FLASH_EraseBlock(BlockNum, FLASH_MEMTYPE_PROG);
	
			/* Wait until End of high voltage flag is set*/
			while (FLASH_GetFlagStatus(FLASH_FLAG_HVOFF) == RESET)
			{}
			FLASH_ProgramBlock(BlockNum, FLASH_MEMTYPE_PROG, FLASH_PROGRAMMODE_STANDARD, Flash_buf);
			/* Wait until End of high voltage flag is set*/
			while (FLASH_GetFlagStatus(FLASH_FLAG_HVOFF) == RESET)
			{}
			I2C_IAP_status = I2C_IAP_WRITE_END;
			Rx_Idx = 0;
			break;
		}
		case I2C_IAP_WRITE_END:
		{
			I2C_IAP_status = I2C_IAP_NULL;
			break;
		}
		case I2C_IAP_UPDATE_START:
		{
			IAP_Flash_update();
			I2C_IAP_status = I2C_IAP_UPDATE_END;
			break;
		}
		case I2C_IAP_RESET_MCU_START:
		{
			Reset_mcu();
			break;
		}
		case I2C_IAP_NULL:
			break;
		default:
			break;
	
	}

}




#pragma section()

