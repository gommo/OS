/*************************************************************************
 *                          -=-=-= taskm.h =-=-=-
 *
 * THIS FILE WAS CREATED FOR ELEC4710 - REALTIME SYSTEMS
 * MAJOR PROJECT ASSIGNMENT
 *
 * This file contains structures and definitons related to task
 * Management. This includes the Task State Structure.
 *
 * Compiler: gcc
 *
 * Author: Colin Goudie
 **************************************************************************/
#ifndef __TASKM_H__
#define __TASKM_H__

/** Number of Tasks supported by the Operating System */
#define         NUMBER_OF_TASKS                 128

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
    long    previous_task_link;        /* 16 high bits zero */
    long    esp0;       
    long    ss0;                       /* 16 high bits zero */
    long    esp1;                       
    long    ss1;                       /* 16 high bits zero */
    long    esp2;                   
    long    ss2;                       /* 16 high bits zero */
    long    cr3;                    
    long    eip;
    long    eflags;
    long    eax, ecx, edx, ebx;
    long    esp, ebp, esi, edi;
    long    es;                        /* 16 high bits zero */
    long    cs;                        /* 16 high bits zero */
    long    ss;                        /* 16 high bits zero */
    long    ds;                        /* 16 high bits zero */
    long    fs;                        /* 16 high bits zero */
    long    gs;                        /* 16 high bits zero */
    long    io_map_base;               /* bits 1-15 zero 
                                          bit 0 contains Trap flag */
};

#endif
