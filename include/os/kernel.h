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

/** This idea is taken from the linux 0.01 kernel. We set up a 
    user stack but we also use it as the starting kernel stack too */
long    user_stack [ PAGE_SIZE >> 2 ];

/** Initialise a Stack Desscriptor pointing at the top of the user_stack
    (PAGE>>2)  and pointing to our data segment (0x10) */
struct stack start_stack = { &user_stack[PAGE_SIZE >> 2], 0x10 };



#endif

