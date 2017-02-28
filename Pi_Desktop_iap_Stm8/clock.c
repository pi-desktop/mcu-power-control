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
#include "clock.h"

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


static clock_time_t tick = 0;


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
void system_tick(void)
{
	tick ++;
	//GPIO_WriteReverse(GPIOC, GPIO_PIN_1);
}

/*****************************************************************************
 * Description:
 *		add funtion description here
 * Parameters:
 *		description for each argument, new argument starts at new line
 * Return:
 *		what does this function returned?
 *****************************************************************************/
clock_time_t clock_time(void)
{
	return tick;
}

#pragma section()



