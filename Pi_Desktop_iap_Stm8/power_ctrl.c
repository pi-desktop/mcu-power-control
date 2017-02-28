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
#include "timer.h"
#include "led.h"
#include "key.h"

/*****************************************************************************
 * Define section
 * add all #define here
 *****************************************************************************/
#define POWER_STATE_OFF                                           (0)
#define POWER_STATE_ON                                             (1)
#define POWER_STATE_WATI_CLICK_OFF                    (2)
#define POWER_STATE_CLICK_OFF                               (3)
#define POWER_STATE_WATI_GUI_OFF_REBOOT        (4)
#define POWER_STATE_GUI_OFF_REBOOT                   (5)
#define POWER_STATE_WAIT_ON                     (6)

#define OPEN_CLAMP         (0)
#define OPEN_RELEASE      (1)

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

static int power_state = POWER_STATE_OFF;
static struct timer power_timer={0};
#pragma section()

 /* function body */
#pragma section (power_text)

/*****************************************************************************
 * Description:
 *		add funtion description here
 * Parameters:
 *		description for each argument, new argument starts at new line
 * Return:
 *		what does this function returned?
 *****************************************************************************/
void power_init(void)
{
	// set open and close pin

	GPIO_Init(CLOSE_GPIO_PORT, (GPIO_Pin_TypeDef)CLOSE_GPIO_PINS, GPIO_MODE_OUT_PP_LOW_FAST);
	GPIO_WriteLow(CLOSE_GPIO_PORT, CLOSE_GPIO_PINS);

	GPIO_Init(OPEN_GPIO_PORT, (GPIO_Pin_TypeDef)OPEN_GPIO_PINS, GPIO_MODE_IN_PU_NO_IT);

	// set POWER and SATA

	GPIO_Init(POWER_GPIO_PORT, (GPIO_Pin_TypeDef)POWER_GPIO_PINS, GPIO_MODE_OUT_PP_LOW_FAST);
	GPIO_WriteLow(POWER_GPIO_PORT, POWER_GPIO_PINS);

	GPIO_Init(SATA_GPIO_PORT, (GPIO_Pin_TypeDef)SATA_GPIO_PINS, GPIO_MODE_OUT_PP_LOW_FAST);
	GPIO_WriteLow(SATA_GPIO_PORT, SATA_GPIO_PINS);

	power_state = POWER_STATE_OFF;
}


/*****************************************************************************
 * Description:
 *		add funtion description here
 * Parameters:
 *		description for each argument, new argument starts at new line
 * Return:
 *		what does this function returned?
 *****************************************************************************/
static void power_on(void)
{
	GPIO_WriteHigh(POWER_GPIO_PORT, POWER_GPIO_PINS);
}


/*****************************************************************************
 * Description:
 *		add funtion description here
 * Parameters:
 *		description for each argument, new argument starts at new line
 * Return:
 *		what does this function returned?
 *****************************************************************************/
static void power_off(void)
{
	GPIO_WriteLow(POWER_GPIO_PORT, POWER_GPIO_PINS);
}


/*****************************************************************************
 * Description:
 *		add funtion description here
 * Parameters:
 *		description for each argument, new argument starts at new line
 * Return:
 *		what does this function returned?
 *****************************************************************************/
static void SATA_power_on(void)
{
	GPIO_WriteHigh(SATA_GPIO_PORT, SATA_GPIO_PINS);
}


/*****************************************************************************
 * Description:
 *		add funtion description here
 * Parameters:
 *		description for each argument, new argument starts at new line
 * Return:
 *		what does this function returned?
 *****************************************************************************/
static void SATA_power_off(void)
{
	GPIO_WriteLow(SATA_GPIO_PORT, SATA_GPIO_PINS);
}


/*****************************************************************************
 * Description:
 *		add funtion description here
 * Parameters:
 *		description for each argument, new argument starts at new line
 * Return:
 *		what does this function returned?
 *****************************************************************************/
static int get_open_pin(void)
{
	// read gpio
	if(GPIO_ReadInputPin(OPEN_GPIO_PORT, OPEN_GPIO_PINS) == RESET)
		return 0;
	else
		return 1;
}

/*****************************************************************************
 * Description:
 *		add funtion description here
 * Parameters:
 *		description for each argument, new argument starts at new line
 * Return:
 *		what does this function returned?
 *****************************************************************************/
void set_close_pin(void)
{
	volatile int i;
	//GPIO_WriteReverse(CLOSE_GPIO_PORT, CLOSE_GPIO_PINS);
	GPIO_WriteHigh(CLOSE_GPIO_PORT, CLOSE_GPIO_PINS);

	for(i=0;i<500;i++);

	GPIO_WriteLow(CLOSE_GPIO_PORT, CLOSE_GPIO_PINS);
}


/*  Test Code LED  */
/*
void Led_left_on(void)
{
	GPIO_WriteHigh(LED_GPIO_PORT, LED_GPIO_PINS);
}

void Led_left_off(void)
{
	GPIO_WriteLow(LED_GPIO_PORT, LED_GPIO_PINS);
}

void Led_right_on(void)
{
	GPIO_WriteHigh(PI_LED_GPIO_PORT, PI_LED_GPIO_PINS);
}

void Led_right_off(void)
{
	GPIO_WriteLow(PI_LED_GPIO_PORT, PI_LED_GPIO_PINS);
}
*/

/*****************************************************************************
 * Description:
 *		add funtion description here
 * Parameters:
 *		description for each argument, new argument starts at new line
 * Return:
 *		what does this function returned?
 *****************************************************************************/
void power_ctrl(void)
{
	key_value_t key_temp;

	key_temp = key_read();

	switch(power_state){
		case POWER_STATE_OFF:
			if(KEY_CLICK == key_temp){
				// turn on power and leds
				SATA_power_on();
				power_on();
				led_sel_type(LED_STYLE_ON);
				//Led_left_on();
				timer_set(&power_timer,1000);  // wait 1s
				power_state = POWER_STATE_WAIT_ON;
			}
		break;

		case POWER_STATE_WAIT_ON:
			// sleep more than 20us
			if(!timer_expired(&power_timer))
				break;

			if(OPEN_CLAMP == get_open_pin()){
				power_state = POWER_STATE_ON;
			} else if(key_temp == KEY_LONG) {
				//do force shut down
				power_off();
				led_sel_type(LED_STYLE_OFF);
				SATA_power_off();
				power_state = POWER_STATE_OFF;
			} else {

			}
		break;

		case POWER_STATE_ON:
			if(KEY_CLICK_OFF == key_temp){// user click key to trun off
				// trig the close signal
				set_close_pin();
				power_state = POWER_STATE_WATI_CLICK_OFF;
			}else if(OPEN_RELEASE == get_open_pin()){
				// user shut down at GUI or comand line waiting for 10s
				timer_set(&power_timer,30000);
				power_state = POWER_STATE_WATI_GUI_OFF_REBOOT;
			} else {
				// MCU enter low power if needed
			}

		break;

		case POWER_STATE_WATI_CLICK_OFF:
			if((OPEN_RELEASE== get_open_pin()) || (key_temp == KEY_LONG)){
				//turn off power and leds or do force shut down
				power_off();
				led_sel_type(LED_STYLE_OFF);
				SATA_power_off();
				power_state = POWER_STATE_OFF;
			}
		break;

		case POWER_STATE_WATI_GUI_OFF_REBOOT:
			if(!timer_expired(&power_timer) && get_open_pin()!=0){
				led_sel_type(LED_STYLE_BLINK);

				if(key_temp == KEY_LONG) {
					//do force shut down
					power_off();
					led_sel_type(LED_STYLE_OFF);
					SATA_power_off();
					power_state = POWER_STATE_OFF;
				}
				break;
			}

			//clear the timer
			timer_set(&power_timer,0);

			if(0== get_open_pin()){
				power_on();
				led_sel_type(LED_STYLE_ON);
				SATA_power_on();
				power_state = POWER_STATE_ON;

			} else {
				//turn off power and leds
				power_off();
				led_sel_type(LED_STYLE_OFF);
				SATA_power_off();
				power_state = POWER_STATE_OFF;
			}
		break;

		default: ;

	}
}

#pragma section()


