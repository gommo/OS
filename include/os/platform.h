/*************************************************************************
 *                          -=-=-= taskm.h =-=-=-
 *
 * THIS FILE WAS CREATED FOR ELEC4710 - REALTIME SYSTEMS
 * MAJOR PROJECT ASSIGNMENT
 *
 * This file contains structures and definitons specifically related to
 * the i386 architecture platform
 *
 * Compiler: gcc
 *
 *Author: Colin Goudie
 **************************************************************************/
#ifndef __PLATFORM_H__
#define __PLATFORM_H__

/* We will be running our OS in protected mode under a Basic Flat
 * Memory model. In this model the entire addressable (4GB) memory
 * space is available to both code and data segments.
 *
 * Some Notes:
 * Stack Segment is a part of Data Segment
 *
 * We must have a minimum of One Code segment and One Data segment
 * set up.
 *
 * The CS Register is used to access a Descriptor that points to a 
 * Code segment
 * The DS Register is used to access a Descriptor that points to a 
 * Data segment
 * The SS Register is used to access a Descriptor that points to a 
 * Stack segment
 *
 * The ES, FS and GS Registers are additional data segment selectors
 */

/* Code and Data Segment Type Defines. These are structured in such
 * a way as to be able to bitwise OR them to a descriptor field after
 * ANDING WITH THE SEGMENT_TYPE_MASK
 */
#define SEGMENT_TYPE_MASK                           0xf0ff
#define DATA_READONLY                               0xf0ff
#define DATA_READONLY_ACCESSED                      0xf1ff
#define DATA_READWRITE                              0xf2ff
#define DATA_READWRITE_ACCESSED                     0xf3ff
#define DATA_READONLY_EXPD                          0xf4ff
#define DATA_READONLY_EXPD_ACCESSED                 0xf5ff
#define DATA_READWRITE_EXPD                         0xf6ff
#define DATA_READWRITE_EXPD_ACCESSED                0xf7ff
#define CODE_EXECUTEONLY                            0xf8ff
#define CODE_EXECUTEONLY_ACCESSED                   0xf9ff
#define CODE_EXECUTEREAD                            0xfaff
#define CODE_EXECUTEREAD_ACCESSED                   0xfbff
#define CODE_EXECUTEONLY_CONFORMING                 0xfcff
#define CODE_EXECUTEONLY_CONFORMING_ACCESSED        0xfdff
#define CODE_EXECUTEREADONLY_CONFORMING             0xfeff
#define CODE_EXECUTEREADONLY_CONFORMING_ACCESSED    0xffff

/** Page size for this processor */
#define     PAGE_SIZE           4096

/** This structure describes a descriptor used in the 386 processor */
struct descriptor
{
    unsigned long a, b;
};

typedef struct descriptor_table
{
    struct descriptor descriptor;
} desc_table[256];

/*
 * Some inline assembly functions for setting up platforms
 * specific structures
 */

/**
 * This function is used to set up an interrupt gate. See Interrupt Gate on
 * page 153 of the Intel Arch Dev Manual II.
 *
 * @param unsigned int The interrupt number that this gate is
 * @param void* The pointer to the function that handles the interrupt
 */
void inline set_interrupt_gate(unsigned int number, void* address)
{
    asm("movw %%dx, %%ax\n\t"  /* Moves offset 15..0 from dx to ax */
        "movw %2, %%dx\n\t"    /* Moves 0x8E00 into dx */
        "movl %%eax, %0\n\t"
        "movl %%edx, %1\n\t"
        ::  "o" ((char*)(&idt[number])), /* This parameter is the offset into the IDT */
            "o" (4 + (char*)(&idt[number])),
            "i" ((unsigned short)(0x8E00)), /* Hardcoded val for P,DPL, Type */
            "d" (address), /* edx will contain the address of our handler */
            "a" (0x00080000) /* This immediate value will go into eax and it
                                is hardcoded so the high word is the segment
                                selector  */
       );
    
}

                
                
#endif
