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

extern desc_table idt;

/**
 * Below are some functions used to set task gates, interrupt gates and trap
 * gates.
 *
 * For more information about what these are see the Intel Dev Manuals
 * Book 3, Section 5.11
 */

ulong set_idt_task_gate( uint int_number, 
                         ushort selector,
                         ulong offset,
                         uchar dpl,
                         uchar present );

ulong set_idt_interrupt_gate( uint int_number, 
                         ushort selector,
                         ulong offset,
                         uchar dpl,
                         uchar present );

ulong set_idt_trap_gate( uint int_number, 
                         ushort selector,
                         ulong offset,
                         uchar dpl,
                         uchar present );



#endif
