/*************************************************************************
*                            -=-=-= sched.c =-=-=-
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
#include <os/sched.h>

/** Current PID to give out */
static ulong    current_pid = 0;
/** Current TID to give out */
static ulong    current_tid = 0;

