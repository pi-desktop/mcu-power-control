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
#include "led.h"

/*****************************************************************************
 * Define section
 * add all #define here
 *****************************************************************************/
#define LED_OFF         (0)
#define LED_ON          (0)
#define LED_BLINK     (0)


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
#pragma section {power_data}

static led_style_t  led_style_on    = {0};
static led_style_t  led_style_off   = {0};
static led_style_t  led_style_blink = {0};
static led_style_t  led_style_blink2= {0};

#pragma section()


/*****************************************************************************
 * Global variables section - Local
 * define global variables(will be refered only in this file) here,
 * static keyword should be used to limit scope of local variable to this file
 * e.g.
 *	static uint8_t ufoo;
 *****************************************************************************/
#pragma section {power_data}
static int led_state = 0;
static led_style_t *p_style = 0;

 /* function body */

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
void led_sel_type(uint8_t p)
{
	switch(p)
	{
		case LED_STYLE_OFF:
			p_style = &led_style_off;
			break;
		case LED_STYLE_ON:
			p_style = &led_style_on;
			break;
		case LED_STYLE_BLINK:
			p_style = &led_style_blink;
			break;
		case LED_STYLE_BLINK2:
			p_style = &led_style_blink2;
			break;
		default:
			break;
		
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
void LED_init(void)
{
	GPIO_Init(LED_GPIO_PORT, (GPIO_Pin_TypeDef)LED_GPIO_PINS, GPIO_MODE_OUT_PP_LOW_FAST);
	GPIO_WriteLow(LED_GPIO_PORT, LED_GPIO_PINS);

	GPIO_Init(PI_LED_GPIO_PORT, (GPIO_Pin_TypeDef)PI_LED_GPIO_PINS,GPIO_MODE_OUT_PP_LOW_FAST);
	GPIO_WriteLow(PI_LED_GPIO_PORT, PI_LED_GPIO_PINS);
	led_style_on.pointer = 0;
	led_style_off.pointer = 0;
	led_style_blink.pointer = 0;
	led_style_blink2.pointer = 0;
	led_style_on.sty = 0xFFFF;
	led_style_off.sty = 0x0;
	led_style_blink.sty = 0x0200;
	led_style_blink2.sty = 0x0280;
	led_sel_type(LED_STYLE_OFF);
}

/*****************************************************************************
 * Description:
 *		add funtion description here
 * Parameters:
 *		description for each argument, new argument starts at new line
 * Return:
 *		what does this function returned?
 *****************************************************************************/
static void led_on(void)
{
	GPIO_WriteHigh(LED_GPIO_PORT, LED_GPIO_PINS);
	GPIO_WriteHigh(PI_LED_GPIO_PORT, PI_LED_GPIO_PINS);
}

/*****************************************************************************
 * Description:
 *		add funtion description here
 * Parameters:
 *		description for each argument, new argument starts at new line
 * Return:
 *		what does this function returned?
 *****************************************************************************/
void led2_on(void)
{
//	GPIO_WriteHigh(LED_GPIO_PORT, LED_GPIO_PINS);
	GPIO_WriteHigh(PI_LED_GPIO_PORT, PI_LED_GPIO_PINS);
	p_style = &led_style_on;
}

/*****************************************************************************
 * Description:
 *		add funtion description here
 * Parameters:
 *		description for each argument, new argument starts at new line
 * Return:
 *		what does this function returned?
 *****************************************************************************/
void led1_on(void)
{
	GPIO_WriteHigh(LED_GPIO_PORT, LED_GPIO_PINS);
	p_style = &led_style_on;
//	GPIO_WriteHigh(PI_LED_GPIO_PORT, PI_LED_GPIO_PINS);
}

/*****************************************************************************
 * Description:
 *		add funtion description here
 * Parameters:
 *		description for each argument, new argument starts at new line
 * Return:
 *		what does this function returned?
 *****************************************************************************/
static void led_off(void)
{
	GPIO_WriteLow(LED_GPIO_PORT, LED_GPIO_PINS);
	GPIO_WriteLow(PI_LED_GPIO_PORT, PI_LED_GPIO_PINS);
}

/*****************************************************************************
 * Description:
 *		add funtion description here
 * Parameters:
 *		description for each argument, new argument starts at new line
 * Return:
 *		what does this function returned?
 *****************************************************************************/
static void pi_led_on(void)
{
	GPIO_WriteHigh(PI_LED_GPIO_PORT, PI_LED_GPIO_PINS);
}

/*****************************************************************************
 * Description:
 *		add funtion description here
 * Parameters:
 *		description for each argument, new argument starts at new line
 * Return:
 *		what does this function returned?
 *****************************************************************************/
static void pi_led_off(void)
{
	GPIO_WriteLow(PI_LED_GPIO_PORT, PI_LED_GPIO_PINS);
}

/*****************************************************************************
 * Description:
 *		add funtion description here
 * Parameters:
 *		description for each argument, new argument starts at new line
 * Return:
 *		what does this function returned?
 *****************************************************************************/
void led_process(void)
{
	if(((p_style->sty) & (1 << p_style ->pointer)) >= 1)
		led_on();
	else
		led_off();

	if(p_style ->pointer < 9)
		p_style ->pointer ++;
	else
		p_style ->pointer = 0;
}

#pragma section()


