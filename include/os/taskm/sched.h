/*************************************************************************
*                            -=-=-= sched.h =-=-=-
*
* THIS FILE WAS CREATED FOR ELEC4710 - REALTIME SYSTEMS
* MAJOR PROJECT ASSIGNMENT
*
* This file contains code responsible for the scheduling of the processes
* and threads in the operating system
*
* Compiler: gcc
*
* Author: Colin Goudie
**************************************************************************/
#ifndef __SCHED_H__
#define __SCHED_H__

#include <os/config.h>
#include <os/platform.h>
#include <os/mm/paging.h>

/** Number of Processes supported by the Operating System */
#define         MAX_PROCESSES                   128

/** Max size of a process name */
#define         MAX_PROCESS_NAME                256

/** Task state defines */
#define         TASK_RUNNING                    3
#define         TASK_WAITING                    2
#define         TASK_STOPPED                    1
#define         TASK_ZOMBIE                     0

#define         PERMENANT_PROCESS               -1

/**
* Task structure. This structure is probably the most important structure
* in the OS. It is used to store all the important information about 
* a task
*/
struct process
{
    /** Name of the Process */
    char    name[MAX_PROCESS_NAME];
    /** Process ID */
    ulong   pid;
    /** Links to the other processes on this list */
    struct process* next;
    struct process* prev;
    /** Clock ticks to live for this task (-1 is always running)*/
    long   time_to_live;
    /** State of the Task. 0 = zombie, 1 = stopped, 2 = waiting, 3 = running */
    uchar  state;
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
    /** Threads Task State Structure, this is where all the data a thread needs is held */
    struct tss              task_state;
    long                    user_stack[PAGE_SIZE >> 2];
    long                    kernel_stack[PAGE_SIZE >> 2];
};

/**
* Creates a process to run on this operating system
*/
void create_proc(char* task_name, void* function, void* params);
/**
 * Initialises the scheduler and idle process
 */
void init_sched();
/** Returns a new Process ID */
ulong get_pid();
/** Returns a new Thread ID */
ulong get_tid();
/** Returns the idle task */
struct process* get_idle_task();

#endif
