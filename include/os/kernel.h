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


struct stack
{
    long* a;    /* Pointer to the stack data memory */
    short b;    /* Segment Selector                 */
};

void k_clear_screen();
unsigned int k_printf(char *message, unsigned int line);

#endif

