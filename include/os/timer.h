/*************************************************************************
*                             -=-=-= timer.h =-=-=-
*
* THIS FILE WAS CREATED FOR ELEC4710 - REALTIME SYSTEMS
* MAJOR PROJECT ASSIGNMENT
*
* This file contains functions and definitions for the intel 8254 
* programmable interrupt timer. This timer is used to generate interrupts
* to allow us to schedule our tasks.
*
* This file maybe expanded to allow dynamic software timers to be added
* to our system
*
* Compiler: gcc
*
* Author: Colin Goudie
**************************************************************************/
#ifndef __TIMER_H__
#define __TIMER_H__

#include <os/config.h>

#define TIMER_FRQ           100
#define TIMER_CLOCK         1193180

void init_timer(void);
void set_frequency(uint frequency);
void timer_interrupt_handler(void);

#endif
