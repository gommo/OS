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

/** Page size for this processor */
#define     PAGE_SIZE           4096

/**
 * A segment descriptor is a data structure in a GDT or LDT that provides the 
 * processor with the size and location of a segment as well as access control
 * and status information. (See Intel Dev Manual 3 S:3.4.3)
 * This struct describes a segment descriptor. It uses the gcc C extensions to enable 
 * the declaration of indiviual bits
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
