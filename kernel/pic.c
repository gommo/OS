/*************************************************************************
*                            -=-=-= pic.c =-=-=-
*
* THIS FILE WAS CREATED FOR ELEC4710 - REALTIME SYSTEMS
* MAJOR PROJECT ASSIGNMENT
*
* This file contains definitions and defines for control of the intel
* 8259 Programmable Interrupt Controller
*
* Compiler: gcc
*
* Author: Colin Goudie
*************************************************************************/
#include <os/pic.h>
#include <os/kernel.h>
#include <asm/io.h>

static unsigned short irq_mask;

void reprogram_pic(int pic1, int pic2)
{
    /* Ensure interrupts are disabled */
    disable();

    /* Reprogram the pic so IRQ's are at Interrupts above 31) */
    outb_p( ICW1_INIT + ICW1_ICW4, PIC1_COMMAND );
    outb_p( ICW1_INIT + ICW1_ICW4, PIC2_COMMAND );
    outb_p( pic1, PIC1_DATA );
    outb_p( pic2, PIC2_DATA );
    outb_p( 0x4, PIC1_DATA );
    outb_p( 0x2, PIC2_DATA );
    outb_p( ICW4_8086, PIC1_DATA );
    outb_p( ICW4_8086, PIC2_DATA );

    //Mask all the interrupts
    outb_p( 0xff, PIC1_DATA );
    outb_p( 0xff, PIC2_DATA );

    irq_mask = 0xFFFF;
}

void enable_irq( uchar number )
{
    //Setup our mask
    irq_mask &= ~( 1 << number );

    //If IRQ Number is above 7 then ensure we enable the
    //interrupt on our first PIC to generate through to
    //our second PIC
    if (number > 7)
        irq_mask &= ~(1 << 2);

    //Output to our PIC's
    outb( irq_mask & 0x00FF, PIC1_DATA );
    outb( (irq_mask >> 8) & 0x00FF, PIC2_DATA );
}

void disable_irq( uchar number )
{
    //Setup our mask
    irq_mask |= ( 1 << number );

    //If no more IRQ's exist above 7 then shutoff the cascading
    //interrupt on our PIC1
    if ((irq_mask & 0xFF00) == 0xFF00)
        irq_mask |= (1 << 2);

    //Output to our PIC's
    outb( irq_mask & 0x00FF, PIC1_DATA );
    outb( (irq_mask >> 8) & 0x00FF, PIC2_DATA );
}