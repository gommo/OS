/*************************************************************************
 *                          -=-=-= io.h =-=-=-
 *
 * THIS FILE WAS CREATED FOR ELEC4710 - REALTIME SYSTEMS
 * MAJOR PROJECT ASSIGNMENT
 *
 * This file contains the definitions for the outb, inb etc.. x86 IO
 * instructions
 *
 * Compiler: gcc
 *
 * Author: Colin Goudie
 **************************************************************************/
#ifndef __IO_H__
#define __IO_H__

#include <os/config.h>

static void inline outb(uchar value, uint address)
{
    asm("outb   %%al, %%dx"::"a"(value), "d"(address));
}

static void inline outb_p(uchar value, uint address)
{
    asm("outb   %%al, %%dx\n\t"
        "jmp 1f\n\t"
        "1:\t jmp 1f\n\t"
        "1:"::"a"(value),"d"(address));
}

static uchar inline inb( ushort address )
{
    uchar result;

    asm("inb %%dx, %%ax" : "=a" (result) : "d" (address) );

    return result;
}
#endif


