/*************************************************************************
*                            -=-=-= sched.h =-=-=-
*
* THIS FILE WAS CREATED FOR ELEC4710 - REALTIME SYSTEMS
* MAJOR PROJECT ASSIGNMENT
*
* This file contains code responsible for the scheduling of the processes
* and threads in the operating system
*
* Compiler: gcc
*
* Author: Colin Goudie
**************************************************************************/
#ifndef __SCHED_H__
#define __SCHED_H__

#include <os/config.h>

/**
 * Initialises the scheduler and idle process
 */
void init_sched();
/** Returns a new Process ID */
ulong get_pid();
/** Returns a new Thread ID */
ulong get_tid();

#endif
