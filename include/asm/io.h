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

void inline outb(unsigned int value, unsigned int address)
{
    asm("outb   %%al, %%dx"::"a"(value), "d"(address));
}

void inline outb_p(unsigned int value, unsigned int address)
{
    asm("outb   %%al, %%dx\n\t"
        "jmp 1f\n\t"
        "1:\t jmp 1f\n\t"
        "1:"::"a"(value),"d"(address));
}
#endif


