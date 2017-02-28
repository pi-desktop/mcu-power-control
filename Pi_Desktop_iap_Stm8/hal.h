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
#ifndef _HAL_H_
#define _HAL_H_

/****************************************************************************
 * #include section
 *	add #include here if any
 ***************************************************************************/
#include "stm8s.h"
//#include "iostm8.h"

/****************************************************************************
 * #define section
 *	add constant #define here if any
 ***************************************************************************/
#define LED_GPIO_PORT  (GPIOC)
#define LED_GPIO_PINS  (GPIO_PIN_1)
#define PI_LED_GPIO_PORT  (GPIOC)
#define PI_LED_GPIO_PINS  (GPIO_PIN_2)
#define KEY_GPIO_PORT  (GPIOC)
#define KEY_GPIO_PINS  (GPIO_PIN_3)
#define POWER_GPIO_PORT  (GPIOC)
#define POWER_GPIO_PINS  (GPIO_PIN_4)
#define OPEN_GPIO_PORT  (GPIOD)
#define OPEN_GPIO_PINS  (GPIO_PIN_3)
#define CLOSE_GPIO_PORT  (GPIOD)
#define CLOSE_GPIO_PINS  (GPIO_PIN_2)
#define SATA_GPIO_PORT  (GPIOD)
#define SATA_GPIO_PINS  (GPIO_PIN_7)

/****************************************************************************
 * ADT section
 *	add Abstract Data Type definition here
 ***************************************************************************/



/****************************************************************************
 *  extern variable declaration section
 ***************************************************************************/


/****************************************************************************
 *  section
 *	add function prototype here if any
 ***************************************************************************/


#endif

