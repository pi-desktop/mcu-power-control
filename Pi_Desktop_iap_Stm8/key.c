/*****************************************************************************
 *
 *	Copyright(c) MNDZ, 2016. All rights reserved.
 *
 *	Description:
 *
 *	$Source: $
 *	$Author: $
 *	$Revision:$
 *	$Date: $
 *
 *****************************************************************************/

/*****************************************************************************
 *	Include Section
 *	add all #include here
 *****************************************************************************/
#include "hal.h"
#include "key.h"

/*****************************************************************************
 * Define section
 * add all #define here
 *****************************************************************************/
// 5*10ms=50ms,a click must long than 50ms
#define KEY_TH_CLICK (5)
#define KEY_TH_CLICK_OFF (100)
// 100*10ms = 1s,a long press must long than 5s
#define KEY_TH_LONG  (500)


#define KEY_GPIO_PORT  (GPIOC)
#define KEY_GPIO_PINS  (GPIO_PIN_3)

/****************************************************************************
 * ADT section
 *	add definition of user defined Data Type that only be used in this file  here
 ***************************************************************************/


/******************************************************************************
 * Function prototype section
 * add prototypes for all functions called by this file,execepting those
 * declared in header file
 *****************************************************************************/


/*****************************************************************************
 * Global variables section - Exported
 * add declaration of global variables that will be exported here
 * e.g.
 *	int8_t foo;
 ****************************************************************************/


/*****************************************************************************
 * Global variables section - Local
 * define global variables(will be refered only in this file) here,
 * static keyword should be used to limit scope of local variable to this file
 * e.g.
 *	static uint8_t ufoo;
 *****************************************************************************/
#pragma section {power_data}

static key_value_t key_code = KEY_NONE;
static int delay_cnt = 0;


#pragma section()
  
   /* function body */
#pragma section (power_text)


/*****************************************************************************
 * Description:
 *		init the key code and pin configuration
 * Parameters:
 *		description for each argument, new argument starts at new line
 * Return:
 *		what does this function returned?
 *****************************************************************************/
void key_init(void)
{
	key_code = KEY_NONE;
	delay_cnt = 0;
	GPIO_Init(KEY_GPIO_PORT, (GPIO_Pin_TypeDef)KEY_GPIO_PINS, GPIO_MODE_IN_PU_NO_IT);
}

/*****************************************************************************
 * Description:
 *		init the key code and pin configuration
 * Parameters:
 *		description for each argument, new argument starts at new line
 * Return:
 *		what does this function returned?
 *****************************************************************************/
static int get_key_pin(void)
{
	if(RESET == GPIO_ReadInputPin(KEY_GPIO_PORT, KEY_GPIO_PINS))
		return 0;
	else
		return 1;
}

/*****************************************************************************
 * Description:
 *		should run at every 10ms
 * Parameters:
 *		description for each argument, new argument starts at new line
 * Return:
 *		what does this function returned?
 *****************************************************************************/
void key_scan(void)
{
	if(key_code != KEY_NONE)// invalid key has not been read
		return;

	if(0 == get_key_pin()) {
		delay_cnt ++;
		if(KEY_TH_LONG <= delay_cnt) {
			key_code = KEY_LONG;
		} else if(KEY_TH_CLICK_OFF <= delay_cnt && delay_cnt < KEY_TH_LONG) {
			key_code = KEY_CLICK_OFF;
		}
	}
	else {
		if(delay_cnt < KEY_TH_CLICK) {
			return;// skip jitter
		} else if(KEY_TH_CLICK <= delay_cnt && delay_cnt < KEY_TH_CLICK_OFF) {
			key_code = KEY_CLICK;
		}
		delay_cnt = 0;
	}
}

/*****************************************************************************
 * Description:
 *		add funtion description here
 * Parameters:
 *		description for each argument, new argument starts at new line
 * Return:
 *		what does this function returned?
 *****************************************************************************/
key_value_t key_read(void)
{
	key_value_t key;
	if(key_code == KEY_NONE){
		return KEY_NONE;
	} else {
		key =  key_code;
		key_code = KEY_NONE;
		return key;
	}
}

#pragma section()

