/*************************************************************************
 *                         -=-=-= kernel.h =-=-=-
 *
 * THIS FILE WAS CREATED FOR ELEC4710 - REALTIME SYSTEMS
 * MAJOR PROJECT ASSIGNMENT
 *
 * This file contains definitions, structs and constance related directly
 * to the kernel and scheduling
 *
 * Compiler: gcc
 *
 * Author: Colin Goudie
 *************************************************************************/
#ifndef __KERNEL_H__
#define __KERNEL_H__

#include <os/platform.h>

/**
 * Simple structure that represents a stack. This is used in the lss
 * assembly command to load the stack and stack segment selector at the same
 * time
 */
struct stack
{
    long* a;    /* Pointer to the stack data memory */
    short b;    /* Segment Selector                 */
};

void disable();
void enable();
uchar return_interrupt_status();
ulong save_flags();
void restore_flags(ulong flags);
ulong get_system_ticks();
ulong inc_system_ticks();
void create_gdt_segment_descriptor(   uint segment_index,
                                    uint base_address,
                                    uint segment_limit,
                                    uint descriptor_type,
                                    uint privilege_level,
                                    uint present,
                                    uint granularity );

void k_clear_screen();
unsigned int k_printf(char *message, unsigned int line);
void klprintf(uint line, uchar* fmt, ...);


#endif
/*************************************************************************
* "And don't EVER make the mistake that you can design something better 
*  than what you get from ruthless massively parallel trial-and-error 
*  with a feedback cycle. 
*  That's giving your intelligence _much_ too much credit.
*       - Linux Torvalds
*************************************************************************/

