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
#include <asm/io.h>

/** TSS Descriptor created in init.S */
extern struct tss_descriptor tss_desc;
/** Global TSS Structure created in kernel.c */
extern struct tss global_tss;
/** Extern declaration of the user level thread_exit call
    that is used when a thread finishes execution */
extern int thread_exit(); 

/** Head of our Process list*/
static process_t* process_head = NULL;
/** Current running process */
static process_t* current_process = NULL;

/** Head of our Ready to Run Queue (This will always be the idle task)*/ 
//static thread_t* ready_head = NULL;
/** Current thread running */
static thread_t* current_thread = NULL;

/** Head of the realtime priority thread queue */
static thread_t* realtime_priority_head = NULL;
/** Last executing realtime thread */
static thread_t* last_ex_realtime_thread = NULL;
/** Head of the high priority thread queue */
static thread_t* high_priority_head = NULL;
/** Last executiing high priority thread */
static thread_t* last_ex_high_thread = NULL;
/** Head of the Normal priority thread queue */
static thread_t* normal_priority_head = NULL;
/** Last executing normal priority thread */
static thread_t* last_ex_normal_thread = NULL;
/** Head of the Low priority thread queue */
static thread_t* low_priority_head = NULL;
/** Last executing low priority thread */
static thread_t* last_ex_low_thread = NULL;

/** Current PID to give out */
static ulong    current_pid = 0;
/** Current TID to give out */
static ulong    current_tid = 0;
/** Context Switch assembly function */
extern void context_switch( ulong* old_task_state, ulong* new_task_state, uchar handled_new );
/** Our extern idle function task */
extern void idle_task(void* ptr);
/**
 * Switches to a new thread
 *
 * @param thread* Pointer to the new thread
 */
void thread_switch(thread_t* new_thread);
/**
 * Switched to a new thread that hasnt been run before
 * @param thread* Pointer to the new thread
 */
void thread_switch_to_new_thread(thread_t* new_thread);

void init_sched()
{
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
    create_process("Idle Task", &idle_task, NULL, PRIORITY_LOW);
    //Set the is_new flag to false as we manually start this
    get_idle_task()->thread_list->is_new = FALSE;

    global_tss.ss0 = get_idle_task()->thread_list->task_state.ss0;
    global_tss.esp0 = get_idle_task()->thread_list->task_state.esp0;
}

void create_process(char* task_name, void* function, void* params, uint priority)
{
    process_t* proc;
    thread_t* thread;
    proc = (process_t*)k_malloc(sizeof(process_t));
    thread = (thread_t*)k_malloc(sizeof(thread_t));
    // Set the process name
    strcpy(proc->name, task_name);
    proc->pid = get_pid();
    proc->state = TASK_READY;
    proc->thread_list = thread;
    proc->time_to_live = PERMENANT_PROCESS;
    proc->tty_number = NO_TTY;
    proc->next = proc->prev = NULL;
    
    thread->parent_process = proc;
    thread->next = NULL;
    thread->prev = NULL;
    thread->pnext = NULL;
    thread->pprev = NULL;
    thread->snext = NULL;
    thread->sprev = NULL;
    thread->thread_id = get_tid();
    thread->priority = priority;
    thread->task_state.eip = (ulong)(function);
    thread->task_state.cs = USER_CODE;
    thread->task_state.ss = USER_DATA;
    //thread->user_stack[PAGE_SIZE >> 2] = &thread_exit;
    thread->task_state.esp = (ulong)&thread->user_stack[(PAGE_SIZE >> 2)];
    thread->task_state.ds = USER_DATA;
    thread->task_state.es = USER_DATA;
    thread->task_state.fs = USER_DATA;
    thread->task_state.gs = USER_DATA;
    thread->task_state.ss0 = KERNEL_DATA;
    thread->task_state.esp0 =  (ulong)&thread->kernel_stack[PAGE_SIZE >> 2];
    thread->is_new = TRUE;
    thread->handled_new = FALSE;
    thread->sema = NULL;

    //Add the task to our process list
    if (low_priority_head == NULL && priority == PRIORITY_LOW)
    {
        //This is the first task (Idle)
        low_priority_head = thread;
        current_thread = thread;
        last_ex_low_thread = thread;
        process_head = proc;
        current_process = proc;
    }
    else
    {
        thread_t* th_ptr;
        thread_t** th_list;
        //Get a pointer to the head of the correct thread list
        switch(priority)
        {
        case PRIORITY_LOW:
            th_list = &low_priority_head;
            break;
        case PRIORITY_NORMAL:
            th_list = &normal_priority_head;
            break;
        case PRIORITY_HIGH:
            th_list = &high_priority_head;
            break;
        case PRIORITY_REALTIME:
            th_list = &realtime_priority_head;
            break;
        default:
            //MAJOR KERNEL ERROR
            //Add to low
            th_list = &low_priority_head;
        }
                
        process_t* ptr = process_head;
        //Add the process to the process list
        while (ptr->next != NULL)
            ptr = ptr->next;
        //We have the last process on the list
        ptr->next = proc;
        proc->prev = ptr;

        //Add the processes threads to the ready queue
        //Find the tail of the ready queue
        if ((*th_list) != NULL)
        {
            while ((*th_list)->next != NULL)
                (*th_list) = (*th_list)->next;
        }

        th_ptr = proc->thread_list;
        while (th_ptr != NULL)
        {
            if ((*th_list) != NULL)
            {
                //Set the end of thread list to our new thread
                (*th_list)->next = th_ptr;
                //Set out new threads prev to the end of the list
                th_ptr->prev = (*th_list);
            }
            //Set the end of the list to our new thread
            (*th_list) = th_ptr;
            //Get the next thread belonging to this new process
            th_ptr = th_ptr->pnext;
        }
        
    }
}

void schedule()
{
    thread_t* new_thread;

    new_thread = NULL;

    //Search for next task from highest priority to lowest
    if (last_ex_realtime_thread == NULL)
    {
        //There have been no realtime threads running, has one been added?
        if (realtime_priority_head != NULL)
        {
            last_ex_realtime_thread = realtime_priority_head;
            new_thread = last_ex_realtime_thread;
        }
    }
    else
    {
        if (last_ex_realtime_thread->next != NULL)
        {
            last_ex_realtime_thread = last_ex_realtime_thread->next;
            new_thread = last_ex_realtime_thread;
        }
        else
        {
            //Set it to the head
            last_ex_realtime_thread = realtime_priority_head;
            new_thread = last_ex_realtime_thread;
        }
    }
    
    if (new_thread == NULL)
    {
        //Check for high priority tasks
        if (last_ex_high_thread == NULL)
        {
            //If there was no last executing high threads
            if (high_priority_head != NULL)
            {
                last_ex_high_thread = high_priority_head;
                new_thread = last_ex_high_thread;
            }
        }
        else
        {
            if (last_ex_high_thread->next != NULL)
            {
                last_ex_high_thread = last_ex_high_thread->next;
                new_thread = last_ex_high_thread;
            }
            else
            {
                //Set it to the head
                last_ex_high_thread = high_priority_head;
                new_thread = last_ex_high_thread;
            }
        }

    }

    if (new_thread == NULL)
    {
        //Check for normal priority tasks
        if (last_ex_normal_thread == NULL)
        {
            //If there was no last executing high threads
            if (normal_priority_head != NULL)
            {
                last_ex_normal_thread = normal_priority_head;
                new_thread = last_ex_normal_thread;
            }
        }
        else
        {
            if (last_ex_normal_thread->next != NULL)
            {
                last_ex_normal_thread = last_ex_normal_thread->next;
                new_thread = last_ex_normal_thread;
            }
            else
            {
                //Set it to the head
                last_ex_normal_thread = normal_priority_head;
                new_thread = last_ex_normal_thread;
            }
        }   
    }

    if (new_thread == NULL)
    {
        //Check for low priority tasks
        if (last_ex_low_thread == NULL)
        {
            //If there was no last executing high threads
            if (low_priority_head != NULL)
            {
                last_ex_low_thread = low_priority_head;
                new_thread = last_ex_low_thread;
            }
        }
        else
        {
            if (last_ex_low_thread->next != NULL)
            {
                last_ex_low_thread = last_ex_low_thread->next;
                new_thread = last_ex_low_thread;
            }
            else
            {
                //Set it to the head
                last_ex_low_thread = low_priority_head;
                new_thread = last_ex_low_thread;
            }
        }
    }

    if (new_thread->is_new)
    {
        thread_switch_to_new_thread(new_thread);
    }
    else
        thread_switch(new_thread);
}

void thread_switch_to_new_thread(thread_t* new_thread)
{
    asm volatile ("pushal\n\t"                      /* Push all general purpose registers onto stack  */
                  "movl    %0, %%eax\n\t"           /* Put the address of old_task_state in eax                         */
                  "movl    %%esp, 4(%%eax)\n\t"     /* move the current stack pointer into esp0 in task_state structure */
                  "movw    %%ss, 8(%%eax)\n\t"      /* move the current ss into ss0 in old_task_state structure         */
                  ::"m"(&current_thread->task_state));//, "m"(ptr-1));//, "i"(12));

    //Make sure this new process is not new anymore
    new_thread->is_new = FALSE;
    //Set the current threads process to ready
    current_thread->parent_process->state = TASK_READY;
    /** The current thread has handled a new task */
    current_thread->handled_new = TRUE;
    //Set the current thread to this new thread
    current_thread = new_thread;
    current_thread->parent_process->state = TASK_RUNNING;
    current_process = current_thread->parent_process;
    //Set up the global tss to use the new processes kernel stack
    global_tss.ss0 = current_thread->task_state.ss0;
    global_tss.esp0 = current_thread->task_state.esp0;

    //Switch over to the new processes kernel stack
    asm volatile ("movl   %0, %%esp\n\t"
                  "movl   %1, %%ss\n\t"
                  ::"m"(current_thread->task_state.esp0), "m"(current_thread->task_state.ss0));

    /* Signal the end of interrupt */
    outb(0x20, 0x20);
    /* Enable interrupts */
    enable();

    jump_to_ring3_task(current_thread->task_state.ss,
    current_thread->task_state.esp,
    current_thread->task_state.cs,
    current_thread->task_state.eip);
}

void thread_switch(thread_t* new_thread)
{
    /** Local copy of current thread */
    thread_t* current = current_thread;
    /** Cache the handled new */
    char handled_new = new_thread->handled_new;
    new_thread->handled_new = FALSE;

    /* Don't bother switching if switching to itself */
    if (new_thread == current_thread)
        return;

    /* Set the current thread to be ready for execution again */
    current_thread->parent_process->state = TASK_READY;
    /* Set the current thread to this new thread */
    current_thread = new_thread;
    /* Set the current process to state running */
    current_thread->parent_process->state = TASK_RUNNING;
    /* Update our global current_process to this process */
    current_process = current_thread->parent_process;

    context_switch( (ulong*)&current->task_state, (ulong*)&new_thread->task_state, handled_new );
     
}

int sys_thread_exit()
{
    //Need to remove this current thread 

    //Probably set current_thread to the null thread then perform a task switch
    return SUCCESS;
}

process_t* get_idle_task()
{
    return process_head;
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
    return current_process->name;
}

process_t* get_current_task()
{
    return current_process;
}

thread_t* get_current_thread()
{
    return current_thread;
}

