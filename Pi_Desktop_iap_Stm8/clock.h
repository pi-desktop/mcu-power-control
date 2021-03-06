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
#ifndef _CLOCK_H_
#define _CLOCK_H_

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
typedef unsigned long clock_time_t;


/****************************************************************************
 *  extern variable declaration section
 ***************************************************************************/


/****************************************************************************
 *  section
 *	add function prototype here if any
 ***************************************************************************/
extern void system_tick(void);
extern clock_time_t clock_time(void);

#endif

