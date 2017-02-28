/*****************************************************************************
 *
 *	Copyright(c) MNDZ, 2008. All rights reserved.
 *
 *	Description:
 *
 *	$Source: $
 *	$Author: $
 *	$Revision:$
 *	$Date: $
 *
 *****************************************************************************/
#ifndef _KEY_H_
#define _KEY_H_

/****************************************************************************
 * #include section
 *	add #include here if any
 ***************************************************************************/
#include "stm8s.h"
#include "iostm8.h"

/****************************************************************************
 * #define section
 *	add constant #define here if any
 ***************************************************************************/

/****************************************************************************
 * ADT section
 *	add Abstract Data Type definition here
 ***************************************************************************/
typedef enum key_value{
	KEY_NONE = 0,
	KEY_CLICK,
	KEY_CLICK_OFF,
	KEY_LONG,
	KEY_DOUBLE
} key_value_t;




/****************************************************************************
 *  extern variable declaration section
 ***************************************************************************/


/****************************************************************************
 *  section
 *	add function prototype here if any
 ***************************************************************************/
extern void key_init(void);
extern void key_scan(void);
extern key_value_t key_read(void);

#endif
