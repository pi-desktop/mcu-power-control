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

#include "timer_framework.h"

/*****************************************************************************
 * Define section
 * add all #define here
 *****************************************************************************/

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

static u8 timer_1ms = 0;
static u8 timer_10ms = 0;
static u8 timer_100ms = 0;

static p_task_t p_task_1ms = (p_task_t)0;
static p_task_t p_task_10ms = (p_task_t)0;
static p_task_t p_task_100ms = (p_task_t)0;
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
static void sw_timer_dec(uint8_t* timer)
{
	if(*timer == 0) return;
	(*timer)--;
}

/*****************************************************************************
 * Description:
 *		add funtion description here
 * Parameters:
 *		description for each argument, new argument starts at new line
 * Return:
 *		what does this function returned?
 *****************************************************************************/
void timer_init(int us)
{
	//set the timer interrupt periodicity as 100us

	// CLK_Configuration
	CLK_DeInit();
	CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);//HSI16MHZ,1·ÖÆµ

	// TIM2_Configuration_set_100us
	TIM2_DeInit();
	TIM2_TimeBaseInit( TIM2_PRESCALER_16,us);
	TIM2_ITConfig( TIM2_IT_UPDATE , ENABLE);
	TIM2_SetCounter(0x0000);
	TIM2_Cmd(ENABLE);

	enableInterrupts();
}

/*****************************************************************************
 * Description:
 *		add funtion description here
 * Parameters:
 *		description for each argument, new argument starts at new line
 * Return:
 *		what does this function returned?
 *****************************************************************************/
void timer_interrupt(void)
{
	//// interrupt period set to 100us
	//GPIO_WriteReverse(GPIOC, GPIO_PIN_1);
	timer_1ms ++;
}

/*****************************************************************************
 * Description:
 *		be called in main's while(1)
 * Parameters:
 *      none
 * Return:
 *      none
 *****************************************************************************/
void period_tasks(void)
{
	if(timer_1ms < 10){
		return;
	}

	disableInterrupts();
	timer_1ms = 0;
	enableInterrupts();

	(*p_task_1ms)();

	sw_timer_dec(&timer_10ms);
	if(timer_10ms > 0)
		return;
	timer_10ms = 10;
	(*p_task_10ms)();

	sw_timer_dec(&timer_100ms);
	if(timer_100ms > 0)
		return;
	timer_100ms = 10-1;
	(*p_task_100ms)();

}
/*****************************************************************************
 * Description:
 *		add a function in 100ms software timer loop
 * Parameters:
 *		task: function name
 * Return:
 *		none
 *****************************************************************************/
void register_1ms_task(p_task_t task)
{
	p_task_1ms = task;
}

/*****************************************************************************
 * Description:
 *		add a function in 10ms software timer loop
 * Parameters:
 *		task: function name
 * Return:
 *		none
 *****************************************************************************/
void register_10ms_task(p_task_t task)
{
	p_task_10ms = task;
}

 /*****************************************************************************
 * Description:
 *		add a function in 100ms software timer loop
 * Parameters:
 *		task: function name
 * Return:
 *		none
 *****************************************************************************/
void register_100ms_task(p_task_t task)
{
	p_task_100ms = task;
}
#pragma section()

