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

#include <os/config.h>
#include <os/platform.h>

/** Number of Processes supported by the Operating System */
#define         MAX_PROCESSES                   128

/** Max size of a process name */
#define         MAX_PROCESS_NAME                256

/** Task state defines */
#define         TASK_RUNNING                    3
#define         TASK_WAITING                    2
#define         TASK_STOPPED                    1
#define         TASK_ZOMBIE                     0

/**
 * Task structure. This structure is probably the most important structure
 * in the OS. It is used to store all the important information about 
 * a task
 */
struct process
{
    /** Name of the Process */
    char    name[MAX_PROCESS_NAME];
    /** State of the Task. 0 = zombie, 1 = stopped, 2 = waiting, 3 = running */
    uchar           state;
    /** Pointer to the list of threads that make up this process */
    struct thread*      thread_list;
    /** The TTY number that this process can input/output to (-1 if no tty)*/
    int     tty_number;
};

/** 
 * Thread structure
 *
 * @TODO Will add Semaphore pointers etc when we get to it
 */
struct thread
{
    /* Thread ID */
    ulong                   thread_id;
    /* The links to create a thread list */
    struct thread*          next;
    struct thread*          prev;

    /** Threads Stack Frame, this is where all the data a thread needs is held */
    struct stack_frame      stck_frame;
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
 * Initialises task management. 
 */
void init_taskm(void);

#endif
