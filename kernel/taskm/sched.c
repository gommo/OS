/*************************************************************************
*                            -=-=-= sched.c =-=-=-
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
#include <string.h>
#include <os/taskm/sched.h>
#include <os/mm/mm.h>
#include <os/kernel.h>
#include <os/tty.h>

/** TSS Descriptor created in init.S */
extern struct tss_descriptor tss_desc;
/** Global TSS Structure created in kernel.c */
extern struct tss global_tss;
/** Head of our High priority process list */
static struct process* head_hp = NULL;
/** Current running process */
static struct process* current = NULL;
/** Current PID to give out */
static ulong    current_pid = 0;
/** Current TID to give out */
static ulong    current_tid = 0;
/** Our extern idle function task */
extern void idle_task(void* ptr);
/**
 * Switches to a new process
 *
 * @param process* Pointer to the new process
 */
void process_switch(struct process* new_process);

void init_sched()
{
    uint temp;
    //We have to set up a Task Descriptor in the GDT (Move this to a method call)
    tss_desc.base_address_15_00 = (ushort)(((ulong)(&global_tss)) & 0x0000FFFF);
    tss_desc.base_address_23_16 = (uchar)((((ulong)(&global_tss)) & 0x00FF0000) >> 16);
    tss_desc.base_address_31_42 = (uchar)((((ulong)(&global_tss)) & 0xFF000000) >> 24);
    tss_desc.avl = 0x0;
    tss_desc.zero1 = 0x0;
    tss_desc.zero2 = 0x0;
    tss_desc.dpl = 0x0;
    tss_desc.granularity = 0x1;
    tss_desc.type = 0x9;
    tss_desc.present = 0x1;
    tss_desc.segment_limit_15_00 = sizeof(struct tss) & 0xFFFF;
    tss_desc.segment_limit_19_16 = sizeof(struct tss) >> 16;
/*
    asm volatile ("movl %%esp, %%eax\n\t" : "=r" (temp) :: "memory");
    global_tss.ss0 = KERNEL_DATA;
    global_tss.esp0 = temp;

    asm volatile ("ltr %%ax\n\t" :: "a" (SINDEX_TSS << 3));
*/
    asm volatile ("ltr %%ax\n\t" :: "a" (SINDEX_TSS << 3));
    // Just to test and start I'm going to make the user code segments the whole
    // memory space too (16MB) (Obviously dumb)

    //Create the User Code Segment
    create_gdt_segment_descriptor( SINDEX_USER_CODE, 
        0x0000000,
        0x1000,
        EXECUTE_READ_CODE_SEGMENT,
        USER_LEVEL,
        1, 1 );

    // Create the User Data Segment
    create_gdt_segment_descriptor( SINDEX_USER_DATA, 
        0x0000000,
        0x1000,
        READ_WRITE_DATA_SEGMENT,
        USER_LEVEL,
        1, 1 );

    //Create our idle task
    create_proc("Idle Task", &idle_task, NULL);

    global_tss.ss0 = get_idle_task()->thread_list->task_state.ss0;
    global_tss.esp0 = get_idle_task()->thread_list->task_state.esp0;
}

void create_proc(char* task_name, void* function, void* params)
{
    struct process* proc;
    struct thread* thread;
    proc = (struct process*)k_malloc(sizeof(struct process));
    thread = (struct thread*)k_malloc(sizeof(struct thread));
    // Set the process name
    strcpy(proc->name, task_name);
    proc->pid = get_pid();
    proc->state = TASK_WAITING;
    proc->thread_list = thread;
    proc->time_to_live = PERMENANT_PROCESS;
    proc->tty_number = NO_TTY;
    proc->next = proc->prev = NULL;

    thread->next = NULL;
    thread->prev = NULL;
    thread->thread_id = get_tid();
    thread->task_state.eip = (ulong)(function);
    thread->task_state.cs = USER_CODE;
    thread->task_state.ss = USER_DATA;
    thread->task_state.esp = (ulong)&thread->user_stack[PAGE_SIZE >> 2];
    thread->task_state.ds = USER_DATA;
    thread->task_state.es = USER_DATA;
    thread->task_state.fs = USER_DATA;
    thread->task_state.gs = USER_DATA;
    thread->task_state.ss0 = KERNEL_DATA;
    thread->task_state.esp0 =  (ulong)&thread->kernel_stack[PAGE_SIZE >> 2];

    //Add the task to our process list
    if (head_hp == NULL)
    {
        //This is the first task 
        head_hp = proc;
        current = proc;
    }
    else
    {
        struct process* ptr = head_hp;
        while (ptr->next != NULL)
            ptr = ptr->next;
        //We have the last process on the list
        ptr->next = proc;
        proc->prev = ptr;
    }
}

void schedule()
{
    struct process* new_process;
    if (current->next == NULL)
    {
        new_process = head_hp;
    }
    else
    {
        new_process = current->next;
    }
    process_switch(new_process);
}

void process_switch(struct process* new_process)
{
    //Don't bother switching if there is only one task
   /* if (new_process == current)
        return;

    //Set up the global tss to use the new processes kernel stack
    global_tss.ss0 = new_process->thread_list->task_state.ss0;
    global_tss.esp0 = new_process->thread_list->task_state.esp0;
*/
    /*asm("movl   %%esp, %0\n\t"
        "movl   %%ss, %2\n\t"
        "movl   %%)*/
}

struct process* get_idle_task()
{
    return head_hp;
};

ulong get_pid()
{
    return current_pid++;
}

ulong get_tid()
{
    return current_tid++;
}

char* get_current_task_name()
{
    return current->name;
}


