/*************************************************************************
*                             -=-=-= idt.c =-=-=-
*
* THIS FILE WAS CREATED FOR ELEC4710 - REALTIME SYSTEMS
* MAJOR PROJECT ASSIGNMENT
*
* This file contains the function definitions for those declared in idt.h.
* This files main task is to setup the interrupts and their handlers.
*
* Compiler: gcc
*
* Author: Colin Goudie
**************************************************************************/
#include <os/idt.h>
#include <os/kernel.h>

/* defined in system_call.S */
extern void default_interrupt(void);

/**
* This function is used to set up an interrupt handler and descriptor. It
* inserts into the IDT a correctly formatted descriptor to handle the
* inputted interrupt number.
*
* @param GATE_TYPE Type of the interrupt descriptor
* @param ushort    Number of the interrupt
* @param void*     Address of the handling function
* @param ushort    The segment selector specifies the code segment to be
*                  used for the handling function
* @param ushort    The privilege level of the handling code
*/
static BOOL set_interrupt_handler(  GATE_TYPE type,
                                    ushort    interrupt_number,
                                    void*     handler_address,
                                    ushort    segment_selector,
                                    ushort    privilege_level);

BOOL init_idt()
{
    int i;

    klprintf(3, "&idt[0] = 0x%08x", &idt[0]);
    klprintf(4, "&idt[1] = 0x%08x", &idt[1]);
    klprintf(5, "&default_interrupt = 0x%08x", &default_interrupt);

    //First set all our interrupts to the default handler 
    for (i=0; i < NUMBER_OF_INTERRUPTS; i++)
    {
        set_interrupt_handler( INTERRUPT_GATE, i, &default_interrupt, KERNEL_CODE, KERNEL_LEVEL);
    }
    
    return TRUE;
}

static BOOL set_interrupt_handler(  GATE_TYPE type,
                                    ushort    interrupt_number,
                                    void*     handler_address,
                                    ushort    segment_selector,
                                    ushort    privilege_level)
{
    static int count = 0;
    klprintf(10, "set_interrupt_handler called %d times", ++count);
    switch(type)
    {
    case TASK_GATE:
        idt[interrupt_number].descripts.tsk_gate.reserved0 = 0x0;
        idt[interrupt_number].descripts.tsk_gate.tss_segment_selector = segment_selector;
        idt[interrupt_number].descripts.tsk_gate.reserved1 = 0x1;
        idt[interrupt_number].descripts.tsk_gate.type = type;
        idt[interrupt_number].descripts.tsk_gate.dpl = privilege_level;
        idt[interrupt_number].descripts.tsk_gate.present = 0x1;
        idt[interrupt_number].descripts.tsk_gate.reserved3 = 0x0;
        break;
    case INTERRUPT_GATE:
        klprintf(11, "set_interrupt_handler (INTERRUPT) called %d times", count);    
        idt[interrupt_number].descripts.int_trp_cll_gate.offset_15_00 = (ushort)(0x0000FFFF & (uint)handler_address);
        idt[interrupt_number].descripts.int_trp_cll_gate.segment_selector = segment_selector;
        idt[interrupt_number].descripts.int_trp_cll_gate.param_count = 0x0;
        idt[interrupt_number].descripts.int_trp_cll_gate.zeros = 0x0;
        idt[interrupt_number].descripts.int_trp_cll_gate.type = type;
        idt[interrupt_number].descripts.int_trp_cll_gate.zero2 = 0x0;
        idt[interrupt_number].descripts.int_trp_cll_gate.dpl = privilege_level;
        idt[interrupt_number].descripts.int_trp_cll_gate.present = 0x1;
        idt[interrupt_number].descripts.int_trp_cll_gate.offset_31_16 = (ushort)((uint)handler_address >> 16);
        break;
    case TRAP_GATE:
        idt[interrupt_number].descripts.int_trp_cll_gate.offset_15_00 = (ushort)(0x0000FFFF & (uint)handler_address);
        idt[interrupt_number].descripts.int_trp_cll_gate.segment_selector = segment_selector;
        idt[interrupt_number].descripts.int_trp_cll_gate.param_count = 0x0;
        idt[interrupt_number].descripts.int_trp_cll_gate.zeros = 0x0;
        idt[interrupt_number].descripts.int_trp_cll_gate.type = type;
        idt[interrupt_number].descripts.int_trp_cll_gate.zero2 = 0x0;
        idt[interrupt_number].descripts.int_trp_cll_gate.dpl = privilege_level;
        idt[interrupt_number].descripts.int_trp_cll_gate.present = 0x1;
        idt[interrupt_number].descripts.int_trp_cll_gate.offset_31_16 = (ushort)((uint)handler_address >> 16);
        break;
    case CALL_GATE:
        idt[interrupt_number].descripts.int_trp_cll_gate.offset_15_00 = (ushort)(0x0000FFFF & (uint)handler_address);
        idt[interrupt_number].descripts.int_trp_cll_gate.segment_selector = segment_selector;
        idt[interrupt_number].descripts.int_trp_cll_gate.param_count = 0x0;
        idt[interrupt_number].descripts.int_trp_cll_gate.zeros = 0x0;
        idt[interrupt_number].descripts.int_trp_cll_gate.type = type;
        idt[interrupt_number].descripts.int_trp_cll_gate.zero2 = 0x0;
        idt[interrupt_number].descripts.int_trp_cll_gate.dpl = privilege_level;
        idt[interrupt_number].descripts.int_trp_cll_gate.present = 0x1;
        idt[interrupt_number].descripts.int_trp_cll_gate.offset_31_16 = (ushort)((uint)handler_address >> 16);
        break;
    default:
        return FALSE;
        break;
    }

    return TRUE;
}

void default_interrupt_handler()
{
    k_printf("default_exception_handler called", 15);
}
