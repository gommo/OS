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
#include <os/taskm/sched.h>

/** Saves the last time the scheduler was called */
static ulong last_do_tick = 0;

extern void check_sleeping_tasks(ulong ticks);

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
    ulong temp = inc_system_ticks();

    //Check sleeping tasks
    check_sleeping_tasks(temp);

    if (temp > last_do_tick)
    {
        last_do_tick = get_system_ticks();
        schedule();
    }

    //Signal the end of interrupt
    outb(0x20, 0x20);
    //Enable interrupts
    enable();
}

