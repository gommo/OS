/*************************************************************************
 *                           -=-=-= idt.h =-=-=-
 *
 * THIS FILE WAS CREATED FOR ELEC4710 - REALTIME SYSTEMS
 * MAJOR PROJECT ASSIGNMENT
 *
 * This file contains functions for interrupt handling and functions that add
 * handlers to our Interrupt Descriptor Table
 *
 * Compiler: gcc
 *
 * Author: Colin Goudie
 **************************************************************************/
#ifndef __IDT_H__
#define __IDT_H__

#include <os/config.h>
#include <os/platform.h>

/* Number of interrupts we support. Changing this will require changes to
   init.S as well */
#define NUMBER_OF_INTERRUPTS        256

/** 
 * idt actually is defined in init.S
 */
extern desc_table idt;

/**
 * This will be the default exception handler that simply displays a message
 * Replace this later with a good panic function that can display some
 * useful information about the error
 */
void default_interrupt_handler();

/**
* Initialises the Interrupt Descriptor Table to contain valid descriptors.
* These are used when interrupts and exceptions occur and jump to valid
* function handlers
*
* @return BOOL true if 
*/
BOOL init_idt();


#endif
