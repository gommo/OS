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

/* Kernel Code Segment Selector Value */
#define     KERNEL_CODE          0x8
#define     KERNEL_DATA          0x10

/* Defines for interrupt enabling/disabling */
#define     disable()           asm volatile ("cli")
#define     enable()            asm volatile ("sti")

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
    uint    error_code;             //Error code

    uint    eip, cs;                //Calling codes cs:ip
    uint    flags;                  //Eflags register
    uint    esp, ss;                //Calling codes ss:esp

};

void k_clear_screen();
unsigned int k_printf(char *message, unsigned int line);
void klprintf(uint line, uchar* fmt, ...);

#endif

