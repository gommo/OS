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

struct stack
{
    long* a;    /* Pointer to the stack data memory */
    short b;    /* Segment Selector                 */
};

/**
 * This structure is used to represent the 
 * information on the stack that is passed
 * to the main interrupt handler function
 */
struct handler_stack_frame
{
    uint    ds, es, fs, gs;         //Segment Registers
    uint    edi, esi;               //Source & destination registers
    uint    ebp, old_esp;           //Base and Stack pointers
    uint    ebx, edx, ecx, eax;     //General purpose registers
    
    uint    interrupt_number;       //Interrupt Number

    /* These values are first pushed onto the stack by the process
       of the processor doing an interrupt. See Intel Dev Manual 
       #1 S:6.4.1 (Fig 6-5)
     */
    uint    error_code;             //Error code
    uint    eip, cs;                //Calling codes cs:ip
    uint    flags;                  //Eflags register
    uint    esp, ss;                //Calling codes ss:esp

};

/**
 * Disables interrupts
 */
void disable();
/**
 * Enables Interrupts
 */
void enable();
/**
 * Returns the status of interrupts
 */
uchar return_interrupt_status();
/**
 * Saves the EFlags register by returning a ulong
 *
 * @return ulong contents of the EFlags register
 */
ulong save_flags();
/**
 * Restores the EFlags register
 *
 * @param ulong new Contents of the EFlags register
 */
void restore_flags(ulong flags);
/**
 * Returns the number of timer ticks the kernel has done 
 *
 * @return ulong number of ticks
 */
ulong get_system_ticks();
/**
 * Increments the number of kernel ticks by one
 */
void inc_system_ticks();
/**
 * This creates a segment descriptor and adds it
 * to the GDT
 *
 * @param uint Segment index into the GDT (Not the selector)
 * @param uint Base address of the segment
 * @param uint Segment Limit
 * @param uint Descriptor Type
 * @param uint Privilege level of the segment
 * @param uint Present flag
 * @param uint Granularity of the segment
 */
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

