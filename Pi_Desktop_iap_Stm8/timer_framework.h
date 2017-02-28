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
#ifndef _TIMER_FRAMEWORK_H_
#define _TIMER_FRAMEWORK_H_

/****************************************************************************
 * #include section
 *	add #include here if any
 ***************************************************************************/
#include "stm8s.h"
/****************************************************************************
 * #define section
 *	add constant #define here if any
 ***************************************************************************/


/****************************************************************************
 * ADT section
 *	add Abstract Data Type definition here
 ***************************************************************************/
typedef void (*p_task_t)(void);

/****************************************************************************
 *  extern variable declaration section
 ***************************************************************************/


/****************************************************************************
 *  section
 *	add function prototype here if any
 ***************************************************************************/
extern void period_tasks(void);
extern void timer_interrupt(void);
extern void timer_init(int us);
extern void register_1ms_task(p_task_t task);
extern void register_10ms_task(p_task_t task);
extern void register_100ms_task(p_task_t task);

#endif
