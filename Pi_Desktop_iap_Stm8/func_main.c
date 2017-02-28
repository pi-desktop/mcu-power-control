

/* Includes ------------------------------------------------------------------*/
#include "hal.h"
//#include "iostm8.h"

#include "hal.h"
#include "led.h"
#include "timer_framework.h"
#include "key.h"
#include "timer.h"
#include "power_ctrl.h"
#include "i2c_slave.h"
#include "stm8s_i2c.h"
#include "iap_flash.h"

#pragma section {power_data}

#pragma section()
//
#pragma section (main)


#if  1

void func_main(void)
{
	int i,j;

	timer_init(100);
	GPIO_DeInit(GPIOD);
	GPIO_DeInit(GPIOC);
	
	power_init();

	LED_init();
	key_init();

	// process register
	register_1ms_task(system_tick);
	register_10ms_task(key_scan);
	register_100ms_task(led_process);

	//led_sel_type(&led_style_off);
	//GPIO_WriteHigh(POWER_GPIO_PORT, POWER_GPIO_PINS);
	//led_sel_type(LED_STYLE_BLINK);

	pid_i2c_init();

	IAP_Flash_init();

	while(1) {
		// execute periodicity job
		//period_tasks();
		IAP_Flash_main();

	}
}

#endif



#pragma section()
/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
