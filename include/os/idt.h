/*************************************************************************
 *                           -=-=-= idt.h =-=-=-
 *
 * THIS FILE WAS CREATED FOR ELEC4710 - REALTIME SYSTEMS
 * MAJOR PROJECT ASSIGNMENT
 *
 * This file contains functions for interrupt handling and functions that add
 * handlers to our Interrupt Descriptor Table
 *
 * Compiler: gcc
 *
 * Author: Colin Goudie
 **************************************************************************/
#ifndef __IDT_H__
#define __IDT_H__

#include <os/config.h>
#include <os/platform.h>

/* Number of interrupts we support. Changing this will require changes to
   init.S as well */
#define NUMBER_OF_INTERRUPTS        256

/* 18 cpu reserved errors */
#define DIVIDE_ERROR                0
#define DEBUG_EXCEPTION             1
#define NMI_HARDWARE                2
#define DEBUG_BREAKPOINT            3
#define INSTRUCTION_OVERFLOW        4
#define INSTRUCTION_OVERRANGE       5
#define INVALID_OPCODE              6
#define NO_COPROCESSOR              7
#define DOUBLE_FAULT                8
#define COPROCESSOR_SEG_OVERRUN     9
#define INVALID_TSS                 10
#define SEGMENT_NOT_PRESENT         11
#define STACK_FAULT                 12
#define GENERAL_PROTECTION_FAULT    13
#define PAGE_FAULT                  14
#define RESERVED15                  15
#define COPROCESSOR_ERROR           16
#define ALIGNMENT_CHECK             17
#define MACHINE_CHECK               18

/* IRQ's */
#define TIMER_IRQ                   0x20

#define SYSTEM_CALL                 0x80

/** 
 * idt actually is defined in init.S
 */
extern desc_table idt;

/**
 * The following externs are functions defined in system_call.S
 */
extern void divide_error();
extern void debug_exception();
extern void nmi_hardware();
extern void debug_breakpoint();
extern void instruction_overflow();
extern void instruction_overrange();
extern void invalid_opcode();
extern void no_coprocessor();
extern void double_fault();
extern void coprocessor_segment_overrun();
extern void invalid_tss();
extern void segment_not_present();
extern void stack_fault();
extern void general_protection_fault();
extern void page_fault();
extern void reserved15();
extern void coprocessor_error();
extern void alignment_check();
extern void machine_check();

extern void irq0(void);
extern void system_call_entry(void);

void main_interrupt_handler(uint ptr_to_stack);
void default_interrupt_handler();
BOOL init_idt();


#endif
