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

/* 18 cpu reserved errors */
#define DIVIDE_ERROR                0
#define DEBUG_EXCEPTION             1
#define NMI_HARDWARE                2
#define DEBUG_BREAKPOINT            3
#define INSTRUCTION_OVERFLOW        4
#define INSTRUCTION_OVERRANGE       5
#define INVALID_OPCODE              6
#define NO_COPROCESSOR              7
#define DOUBLE_FAULT                8
#define COPROCESSOR_SEG_OVERRUN     9
#define INVALID_TSS                 10
#define SEGMENT_NOT_PRESENT         11
#define STACK_FAULT                 12
#define GENERAL_PROTECTION_FAULT    13
#define PAGE_FAULT                  14
#define RESERVED15                  15
#define COPROCESSOR_ERROR           16
#define ALIGNMENT_CHECK             17
#define MACHINE_CHECK               18

/* IRQ's */
#define TIMER_IRQ                   0x20

#define SYSTEM_CALL                 0x80

/** 
 * idt actually is defined in init.S
 */
extern desc_table idt;

/**
 * The following externs are functions defined in system_call.S
 */
extern void irq0(void);
extern void system_call_entry(void);

/**
 * This function handles most interrupts by finding out the interrupt #
 * and calling the appropriate handler functions
 *
 * @param uint Pointer to top of stack
 */
void main_interrupt_handler(uint ptr_to_stack);

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
