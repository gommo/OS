/*************************************************************************
*                             -=-=-= timer.c =-=-=-
*
* THIS FILE WAS CREATED FOR ELEC4710 - REALTIME SYSTEMS
* MAJOR PROJECT ASSIGNMENT
*
* This file contains functions and definitions for the intel 8254 
* programmable interrupt timer. This timer is used to generate interrupts
* to allow us to schedule our tasks.
*
* Compiler: gcc
*
* Author: Colin Goudie
**************************************************************************/
#include <os/timer.h>
#include <os/kernel.h>
#include <asm/io.h>

#define TIMER_FRQ           100
#define TIMER_CLOCK         1193180

void init_timer()
{
    //Set our timers frequency
    set_frequency(TIMER_FRQ);
}

void set_frequency(uint frequency)
{
    ulong latch, flags;
    uchar interrupts_enabled;
    interrupts_enabled = return_interrupt_status();
    latch = TIMER_CLOCK / frequency;
    
    disable();

    flags = save_flags();

    /* Output our control word to the Timer */
    outb_p(0x36,0x43);    // Binary, Mode3
    /* Output the initial count to the timer to start it going */
    outb_p(latch & 0xff, 0x40);     //LSB
    outb_p(latch >> 8, 0x40);       //MSB

    restore_flags(flags);

    if (interrupts_enabled)
        enable();
}

void timer_interrupt_handler()
{
    inc_system_ticks();
}
