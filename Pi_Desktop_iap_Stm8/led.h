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
#ifndef _LED_H_
#define _LED_H_

/****************************************************************************
 * #include section
 *	add #include here if any
 ***************************************************************************/

/****************************************************************************
 * #define section
 *	add constant #define here if any
 ***************************************************************************/


/****************************************************************************
 * ADT section
 *	add Abstract Data Type definition here
 ***************************************************************************/
typedef struct led_style {
	int pointer;
	unsigned int sty;
} led_style_t;


/****************************************************************************
 *  extern variable declaration section
 ***************************************************************************/
#define LED_STYLE_OFF    0     
#define LED_STYLE_ON     1
#define LED_STYLE_BLINK  2
#define LED_STYLE_BLINK2 3



/****************************************************************************
 *  section
 *	add function prototype here if any
 ***************************************************************************/
extern void LED_init(void);
extern void led_sel_type(uint8_t p);
extern void led_process(void);
extern void led2_on(void);
extern void led1_on(void);

#endif

