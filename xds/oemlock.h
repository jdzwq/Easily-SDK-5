﻿/**
* \file timing.h
*/
#ifndef TIMING_H
#define TIMING_H

#include "xdsdef.h"


/**
* \brief          timer structure
*/
struct hr_time
{
	unsigned char opaque[32];
};

#ifdef __cplusplus
extern "C" {
#endif

	extern int alarmed;

	/**
	* \brief          Return the CPU cycle counter value
	*/
	EXP_API unsigned long hardclock(void);

	/**
	* \brief          Return the elapsed time in milliseconds
	*
	* \param val      points to a timer structure
	* \param reset    if set to 1, the timer is restarted
	*/
	EXP_API unsigned long get_timer(struct hr_time *val, int reset);

	/**
	* \brief          Setup an alarm clock
	*
	* \param seconds  delay before the "alarmed" flag is set
	*/
	EXP_API void set_alarm(int seconds);

	/**
	* \brief          Sleep for a certain amount of time
	*/
	EXP_API void m_sleep(int milliseconds);

#ifdef __cplusplus
}
#endif


#endif /* timing.h */