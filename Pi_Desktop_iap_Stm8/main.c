#include "iap_flash.h"
#include "i2c_slave.h"
#include "stm8s_flash.h"
#include "stm8s_clk.h"
#include "stm8s_iwdg.h"
#include "hal.h"

__I uint8_t compile_data[] = __DATE__;
__I uint8_t compile_time[] = __TIME__;
__I uint8_t version[] = "pi-desktop_iap_rev03";

#ifdef _COSMIC_
 int _fctcpy(char name);
#endif /*_COSMIC_*/


void func_main(void);

void sDelay(uint16_t nCount)
{
    /* Decrement nCount value */
    while (nCount != 0)
    {
        nCount--;
    }
}
uint8_t Flash_buffer[128] = {0};
uint16_t Flash_Rx_address;
uint8_t Flash_Rx;


void MoveFromBackToApp(void)
{
	int i;
	uint8_t add;
	uint16_t BlockNum;

	POWER_GPIO_PORT->CR2 &= (uint8_t)(~(POWER_GPIO_PINS));
	POWER_GPIO_PORT->ODR &= (uint8_t)(~(POWER_GPIO_PINS));
	POWER_GPIO_PORT->DDR |= (uint8_t)POWER_GPIO_PINS;
	POWER_GPIO_PORT->CR1 |= (uint8_t)POWER_GPIO_PINS;
	POWER_GPIO_PORT->CR2 &= (uint8_t)(~(POWER_GPIO_PINS));

	LED_GPIO_PORT->CR2 &= (uint8_t)(~(POWER_GPIO_PINS));

	LED_GPIO_PORT->CR2 &= (uint8_t)(~(LED_GPIO_PINS));
	LED_GPIO_PORT->ODR &= (uint8_t)(~(LED_GPIO_PINS));
	LED_GPIO_PORT->DDR |= (uint8_t)LED_GPIO_PINS;
	LED_GPIO_PORT->CR1 |= (uint8_t)LED_GPIO_PINS;
	LED_GPIO_PORT->CR2 &= (uint8_t)(~(LED_GPIO_PINS));

	for(i=0;i<10;i++)
	{
		LED_GPIO_PORT->ODR |= (uint8_t)LED_GPIO_PINS;
		sDelay(0xf000);
		LED_GPIO_PORT->ODR &= (uint8_t)(~LED_GPIO_PINS);
		sDelay(0xf000);
	}


	for(i=0;i<96;i++)
	{
		Flash_Rx = 0;
		for (add = 0; add < 128; add++)
		{
			Flash_Rx_address = 0xa000+128*i+add;
			Flash_buffer[Flash_Rx++] = (*(PointerAttr uint8_t *) (uint16_t)Flash_Rx_address);
		}

		BlockNum = 160+i;
		FLASH_EraseBlock(BlockNum, FLASH_MEMTYPE_PROG);

		while ((FLASH->IAPSR & (uint8_t)FLASH_FLAG_HVOFF) == RESET)
		{}

		FLASH_ProgramBlock(BlockNum, FLASH_MEMTYPE_PROG, FLASH_PROGRAMMODE_STANDARD, Flash_buffer);

		while ((FLASH->IAPSR & (uint8_t)FLASH_FLAG_HVOFF) == RESET)
		{}
		if (i%3)
		{
			LED_GPIO_PORT->ODR |= (uint8_t)LED_GPIO_PINS;
		}
		else
		{
			LED_GPIO_PORT->ODR &= (uint8_t)(~LED_GPIO_PINS);
		}
	}

	for(i=0;i<10;i++)
	{
		LED_GPIO_PORT->ODR |= (uint8_t)LED_GPIO_PINS;
		sDelay(0xf000);
		LED_GPIO_PORT->ODR &= (uint8_t)(~LED_GPIO_PINS);
		sDelay(0xf000);
	}


	*(PointerAttr uint8_t*) (uint16_t)I2C_IAP_FLAG_ADDR = 0;

	IWDG->KR = IWDG_KEY_ENABLE;
	IWDG->KR = (uint8_t)IWDG_WriteAccess_Enable;
	IWDG->PR = (uint8_t)IWDG_Prescaler_4;
	IWDG->RLR = (uint8_t)2;

}

void main(void)
{
	(void)compile_data;
	(void)compile_time;
	(void)version;

	CLK->CKDIVR &= (uint8_t)(~CLK_CKDIVR_HSIDIV);
	CLK->CKDIVR |= (uint8_t)CLK_PRESCALER_HSIDIV1;

#ifdef _COSMIC_
	_fctcpy('F');
#endif /*_COSMIC_*/

	FLASH->CR1 &= (uint8_t)(~FLASH_CR1_FIX);
	FLASH->CR1 |= (uint8_t)FLASH_PROGRAMTIME_STANDARD;

	FLASH->PUKR = FLASH_RASS_KEY1;
	FLASH->PUKR = FLASH_RASS_KEY2;

	/* Wait until Flash Program area unlocked flag is set*/
	while ((FLASH->IAPSR & (uint8_t)FLASH_FLAG_PUL) == RESET)
	{}

	/* Unlock flash data eeprom memory */
	FLASH->DUKR = FLASH_RASS_KEY2; /* Warning: keys are reversed on data memory !!! */
        FLASH->DUKR = FLASH_RASS_KEY1;
	/* Wait until Data EEPROM area unlocked flag is set*/
	while ((FLASH->IAPSR & (uint8_t)FLASH_FLAG_PUL) == RESET)
	{}


	if((*(PointerAttr uint8_t *) (uint16_t)I2C_IAP_FLAG_ADDR) == I2C_IAP_FLAG_VAL)
	{
		MoveFromBackToApp();
	}
	else
	{
		func_main();
	}

	while(1) {

	}
}




/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
