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
#include <asm/io.h>

#define HZ          100
#define LATCH       (1193180/HZ)

void init_timer()
{
    /* Output our control word to the Timer */
    outb_p(0x36,0x43);    // Binary, Mode3
    /* Output the initial count to the timer to start it going */
    outb_p(LATCH & 0xff, 0x40);   //LSB
    outb_p(LATCH >> 8, 0x40);
}

void timer_interrupt_handler()
{

}
