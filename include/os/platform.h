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

#include <os/config.h>

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

/**
 * Privilege Level defines 
 *
 * These defines are used to distinguish between different privilege
 * levels in the operating System
 * (See Intel Dev Manual 3 (S:4.5)
 */
#define LEVEL0          0x0
#define KERNEL_LEVEL    0x0
#define LEVEL1          0x1
#define OS_LEVEL        0x1
#define LEVEL2          0x2
#define LEVEL3          0x3
#define USER_LEVEL       0x3

/* Reference: Intel Arch Dev Manual 3  S:3.4.1-3 */

#define     SINDEX_KERNEL_CODE      1
#define     SINDEX_KERNEL_DATA      2
#define     SINDEX_TSS              3
#define     SINDEX_USER_CODE        4
#define     SINDEX_USER_DATA        5

#define     SINDEX(x)       (x << 3)

#define     TABLE_GDT       (0 << 2)
#define     TABLE_LDT       (1 << 2)

/* Kernel Code Segment Selector Value */
#define     KERNEL_CODE         ( SINDEX( SINDEX_KERNEL_CODE ) + TABLE_GDT + KERNEL_LEVEL )
#define     KERNEL_DATA         ( SINDEX( SINDEX_KERNEL_DATA ) + TABLE_GDT + KERNEL_LEVEL )

#define     USER_CODE           ( SINDEX( SINDEX_USER_CODE ) + TABLE_GDT + USER_LEVEL )
#define     USER_DATA           ( SINDEX( SINDEX_USER_DATA ) + TABLE_GDT + USER_LEVEL )

/**
 * IDT Descriptor and Call Gate Type enum
 * 
 * These enums are used to distinguish between different types of 
 * descriptors
 * (See Intel Dev Manual 3 (S:4.8.3 [Call Gates])
 * (See Intel Dev Manual 3 (S:5.11  [IDT Types])
 */
typedef enum { TASK_GATE = 0x05,
               INTERRUPT_GATE = 0x0E,
               TRAP_GATE = 0x0F,
               CALL_GATE = 0x0C } GATE_TYPE;

/* Code and Data Segment Type Defines. 
 */
#define CODE_SEGMENT                                ( 1 << 3 )
#define DATA_SEGMENT                                ( 0 << 3 )

#define EXPAND_DOWN                                 ( 1 << 2 )
#define NON_EXPAND_DOWN                             ( 0 << 2 )
#define READ_WRITE                                  ( 1 << 1 )
#define READ_ONLY                                   ( 0 << 1 )
#define ACCESSED                                    ( 1 << 0 )
#define NOT_ACCESSED                                ( 0 << 0 )

#define CONFORMING                                  ( 1 << 2 )
#define NON_CONFORMING                              ( 0 << 2 )
#define EXECUTE_ONLY                                ( 0 << 1 )
#define EXECUTE_READ                                ( 1 << 1 )

#define EXECUTE_READ_CODE_SEGMENT       (0x0000 + CODE_SEGMENT + EXECUTE_READ)
#define READ_WRITE_DATA_SEGMENT         (0x0000 + DATA_SEGMENT + READ_WRITE)

/**
* A stack frame contains the necessary information
* to return to an execution space
*/
struct stack_frame
{
    uint eip;
    uint cs;
    uint flags;
    uint esp;
    uint ss;
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
    struct stack_frame stck_frame;  //Stack Frame

};




/* Task-State Segment (TSS) Intel Dev Manual 3 (6.2.1)
*
* The processor state information needed to restore a task is saved
* in a system segment called the task-state segment (TSS). The structure
* contains both dynamic and static fields.
*
* The Following are dynamic fields
* General-Purpose Register Fields
*      State of the EAX, ECX, EDX, EBX, ESP, EBP, ESI and EDI registers
*      prior to the task switch
* Segment Selector fields
*      Segment selectors stroed in the ES, CS, SS, DS, FS and GS registers
*      prior to the task switch
* EFLAGS register field
*      State of the EFLAGS register prior to the task switch
* EIP (Instruction pointer) field
*      State of the EIP register prior to the task switch
* Previous Task Link Field
*      Contains the segment selector for the TSS of the previous task
*      This field premits a task switch back to the previous task to be
*      initiated with an IRET instruction. (This is useful if this task
*      now operating is a interrupt or exception)
*
* The Following are static fields
* LDT segment selector field
*      Contains the segment selector the tasks LDT
* CR3 control register field
*      Contains the base physical address of the page directory to be
*      used by the task. (Also known as the page-directory base register
*      (PDBR))
* Privilega level-0, -1, and -2 stack pointer fields
*      These stack pointers consist of a logical address made up of the
*      segment selector for the stack segment (SS0, SS1, SS2) and an 
*      offset into the stack (ESP0, ESP1, ESP2)
* T (debug trap) flag
*      When set, the T flag causes the processor to raise a debug
*      exception when a task switch to this task occurs
* I/O Map base address field
*      Contains a 16bit offset from the base of the TSS to the I/O
*      permission bit map and interupt redirection bitmap
*/
struct tss
{
    ulong    previous_task_link;        /* 16 high bits zero */
    ulong    esp0;       
    ulong    ss0;                       /* 16 high bits zero */
    ulong    esp1;                       
    ulong    ss1;                       /* 16 high bits zero */
    ulong    esp2;                   
    ulong    ss2;                       /* 16 high bits zero */
    ulong    cr3;                    
    ulong    eip;
    ulong    eflags;
    ulong    eax, ecx, edx, ebx;
    ulong    esp, ebp, esi, edi;
    ulong    es;                        /* 16 high bits zero */
    ulong    cs;                        /* 16 high bits zero */
    ulong    ss;                        /* 16 high bits zero */
    ulong    ds;                        /* 16 high bits zero */
    ulong    fs;                        /* 16 high bits zero */
    ulong    gs;                        /* 16 high bits zero */
    ulong    io_map_base;               /* bits 1-15 zero 
                                        bit 0 contains Trap flag */
};

/**
* This descriptor contains information about locating a tss and
* its state etc..
*/
struct tss_descriptor
{
    ushort  segment_limit_15_00;
    ushort  base_address_15_00;
    uchar   base_address_23_16;
    unsigned type:4;
    unsigned zero1:1;
    unsigned dpl:2;
    unsigned present:1;
    unsigned segment_limit_19_16:4;
    unsigned avl:1;
    unsigned zero2:2;
    unsigned granularity:1;
    uchar   base_address_31_42;
};

/**
 * A segment descriptor is a data structure in a GDT or LDT that provides the 
 * processor with the size and location of a segment as well as access control
 * and status information. (See Intel Dev Manual 3 S:3.4.3)
 * This struct describes a segment descriptor. It uses the gcc C extensions to enable 
 * the declaration of individual bits
 */
typedef struct 
{
    ushort      segment_limit_15_00;
    ushort      base_address_15_00;
    uchar       base_address_23_16;
    unsigned    segment_type:4;
    unsigned    descriptor_type:1;
    unsigned    dpl:2;
    unsigned    present:1;
    unsigned    segment_limit_19_16:4;
    unsigned    avl:1;
    unsigned    zero:1;
    unsigned    default_operation:1;
    unsigned    granularity:1;
    uchar       base_address_31_24;
} /* PACK */ segment_descriptor;

/**
 * The Task gate contains the segment selector for a TSS for an exception
 * and/or interrupt handler task
 */
typedef struct 
{
    ushort      reserved0;
    ushort      tss_segment_selector;
    uchar       reserved1;
    unsigned    type:5;
    unsigned    dpl:2;
    unsigned    present:1;
    ushort      reserved3;
} /* PACK */ task_gate;

/**
 * Call gates and Interrupts controll transfers of program control between
 * different privilege levels.  (Intel Dev Manual 3 S:4.8.3)
 */
typedef struct
{
    ushort      offset_15_00;
    ushort      segment_selector;
    unsigned    param_count:5;
    unsigned    zeros:3;
    unsigned    type:4;
    unsigned    zero2:1;
    unsigned    dpl:2;
    unsigned    present:1;
    ushort      offset_31_16;
} /* PACK */ interrupt_trap_call_gate;

/** 
 * Union that is used to allow the type "descriptor" to be
 * used for any four of the different types of descriptors
 */
typedef union
{
    segment_descriptor seg_descriptor;
    task_gate tsk_gate;
    interrupt_trap_call_gate int_trp_cll_gate;
} descriptor;

typedef struct descriptor_table
{
    descriptor descripts;
} desc_table[256];
           
                
#endif
