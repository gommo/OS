/*************************************************************************
*                            -=-=-= sched.c =-=-=-
*
* THIS FILE WAS CREATED FOR ELEC4710 - REALTIME SYSTEMS
* MAJOR PROJECT ASSIGNMENT
*
* This file contains code responsible for the scheduling of the processes
* and threads in the operating system
*
* The main function in this file is the schedule() function
* Currently this kernel supports four different priority levels
* REALTIME: Has no limit in the time slicing it can be given
* HIGH: Has a high limit to the amount of times it can be scheduled before
*       giving way to a lower priority task
* NORMAL: Has a lower limit to the amount of times it can be scheduled before
*       giving way to a lower prioirty task
* LOW: Lowest form, at the head of this list if the nulltask that runs
*      when other tasks are not ready
*
*
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
#include <os/timer.h>
#include <asm/io.h>

/** TSS Descriptor created in init.S */
extern struct tss_descriptor tss_desc;
/** Global TSS Structure created in kernel.c */
extern struct tss global_tss;
/** Extern declaration of the user level thread_exit call
    that is used when a thread finishes execution */
extern int thread_exit(); 
/** HACK, kernel_stack here is used as a junk stack */
extern long    kernel_stack [ PAGE_SIZE >> 2 ];
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
/** Head of the sleep queue */
static thread_t* sleep_head = NULL;

/** High executing count. */
static uint high_count = 0;
/** Normal executing count */
static uint normal_count = 0;

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
/**
* Initialises the scheduler and idle process
*/
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
    void* temp = create_process("Idle Task", &idle_task, NULL, PRIORITY_LOW);
    klprintf(16, "idle Task lives @ 0x%08x", temp);
    //Set the is_new flag to false as we manually start this
    get_idle_task()->thread_list->is_new = FALSE;

    global_tss.ss0 = get_idle_task()->thread_list->task_state.ss0;
    global_tss.esp0 = get_idle_task()->thread_list->task_state.esp0;
}
/**
* Creates a process to run on this operating system
* 
* @param task_name Name of the process
* @param function Ptr to the initial function of this process
* @param params Parameters to pass to the function (NOT IMPLEMENTED)
* @param priority Priority of this process
* @return Ptr to the main thread of this function (for debug purposes)
*/
void* create_process(char* task_name, void* function, void* params, uint priority)
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
    thread->user_stack[PAGE_SIZE >> 2] = (long)&thread_exit;
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
    thread->wake_up_time = 0;

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
        thread_t* th_list;
        thread_t** th_head;
        //Get a pointer to the head of the correct thread list
        switch(priority)
        {
        case PRIORITY_LOW:
            th_list = low_priority_head;
            th_head = &low_priority_head;
            break;
        case PRIORITY_NORMAL:
            th_list = normal_priority_head;
            th_head = &normal_priority_head;
            break;
        case PRIORITY_HIGH:
            th_list = high_priority_head;
            th_head = &high_priority_head;
            break;
        case PRIORITY_REALTIME:
            th_list = realtime_priority_head;
            th_head = &realtime_priority_head;
            break;
        default:
            //MAJOR KERNEL ERROR
            //Add to low
            th_list = low_priority_head;
            th_head = &low_priority_head;
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
        if (th_list != NULL)
        {
            while (th_list->next != NULL)
                th_list = th_list->next;
        }

        th_ptr = proc->thread_list;
        while (th_ptr != NULL)
        {
            if (th_list != NULL)
            {
                //Set the end of thread list to our new thread
                th_list->next = th_ptr;
                //Set out new threads prev to the end of the list
                th_ptr->prev = th_list;
            }
            else
            {
                //Add to our head
                (*th_head) = th_ptr;
            }
            //Set the end of the list to our new thread
            th_list = th_ptr;
            //Get the next thread belonging to this new process
            th_ptr = th_ptr->pnext;
        }
    }
    return thread;
}
/**
 * Tells the kernel to look for another thread to schedule
 *
 * This currently uses four priority queues, it looks for any
 * highest priority threads first, then lower priority threads.
 */
void schedule()
{
    //Holds the new selected thread
    thread_t* new_thread;
    //If we are about to run the null tasks we check again to 
    //ensure that we could run a higher tasks (This is because, higher tasks
    //can give way to lower tasks but we don't want a higher tasks giving
    //way to the null task
    char recheck = FALSE;
loop:
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
        //Only check for a high prioirty tasks if we have exhausted our limit
        if (high_count < HIGH_PRIORITY_MAX_COUNT)
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
    }

    if (new_thread != NULL)
    {
        //We have picked a high priority thread
        high_count++;
        goto ready_to_thread_switch;
    }

    if (new_thread == NULL)
    {
        //Only check for normal tasks if we haven't exhausted our limit
        if (normal_count < NORMAL_PRIORITY_MAX_COUNT)
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
    }

    if (new_thread != NULL)
    {
        //We have picked a normal priority thread
        normal_count++;
        goto ready_to_thread_switch;
    }

    if (new_thread == NULL)
    {
        //reset the higher priority counts
        normal_count = 0;
        high_count = 0;

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

    if (!recheck && new_thread == low_priority_head)
    {
        //We are about to select the null task
        //check for higher tasks
        recheck = TRUE;
        goto loop;
    }

ready_to_thread_switch:

    if (new_thread->is_new)
    {
        thread_switch_to_new_thread(new_thread);
    }
    else
        thread_switch(new_thread);
}
/**
 * Switches to a new thread that hasn't previously been run yet
 *
 * @param new_thread ptr to the new thread to run
 */
void thread_switch_to_new_thread(thread_t* new_thread)
{
    asm volatile ("pushal\n\t"                      /* Push all general purpose registers onto stack  */
                  "movl    %0, %%eax\n\t"           /* Put the address of old_task_state in eax                         */
                  "movl    %%esp, 4(%%eax)\n\t"     /* move the current stack pointer into esp0 in task_state structure */
                  "movw    %%ss, 8(%%eax)\n\t"      /* move the current ss into ss0 in old_task_state structure         */
                  ::"m"(&current_thread->task_state));//, "m"(ptr-1));//, "i"(12));

    //Make sure this new process is not new anymore
    new_thread->is_new = FALSE;
    //Set the current threads process to ready if it was running
    if (current_thread->parent_process->state == TASK_RUNNING)
    {
        current_thread->parent_process->state = TASK_READY;
    }
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
/**
 * Switches to a new thread
 *
 * @param new_thread ptr to the thread to run
 */
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

    /* Set the current thread to be ready for execution again if it was previously running*/
    if (current_thread->parent_process->state == TASK_RUNNING)
    {
        current_thread->parent_process->state = TASK_READY;
    }
    /* Set the current thread to this new thread */
    current_thread = new_thread;
    /* Set the current process to state running */
    current_thread->parent_process->state = TASK_RUNNING;
    /* Update our global current_process to this process */
    current_process = current_thread->parent_process;

    context_switch( (ulong*)&current->task_state, (ulong*)&new_thread->task_state, handled_new );
     
}
/**
 * Removes the currently running thread from the active running
 * queues.
 */
void remove_current_thread_from_running_queues()
{
    thread_t* thrd = current_thread;
    thread_t** last_ex = NULL;

    //Need to remove this current thread 
    if (thrd->prev == NULL) 
    {
        //We must be the head of our queue
        thread_t* next = thrd->next; //Even if this is null its ok
        switch(thrd->priority) 
        {
        case PRIORITY_REALTIME:
            realtime_priority_head = next;
            break;
        case PRIORITY_HIGH:
            high_priority_head = next;
            break;
        case PRIORITY_NORMAL:
            normal_priority_head = next;
            break;
        case PRIORITY_LOW:
            low_priority_head = next;
            break;
        default:
            //Error don't need to do anything here
            break;
        }

        next->prev = NULL;
    }
    else if (thrd->next == NULL)
    {
        //We must be at the end of our queue
        thrd->prev->next = NULL;
    }
    else
    {
        //We are somewhere in the middle
        //Remove ourselves from the queue
        thrd->prev->next = current_thread->next;
        thrd->next->prev = current_thread->prev;
    }

    //Set the last executing thread for this priority level
    switch(thrd->priority) 
    {
        case PRIORITY_REALTIME:
            last_ex = &realtime_priority_head;
            break;
        case PRIORITY_HIGH:
            last_ex = &high_priority_head;
            break;
        case PRIORITY_NORMAL:
            last_ex = &normal_priority_head;
            break;
        case PRIORITY_LOW:
            last_ex = &low_priority_head;
            break;
        default:
            //Error don't need to do anything here
            break;
    }

    if (thrd->prev != NULL)
    {
        (*last_ex) = thrd->prev;
    }
    else if (thrd->next != NULL)
    {
        (*last_ex) = thrd->next;
    }
    else
    {
        (*last_ex) = NULL;
    }

    thrd->prev = NULL;
    thrd->next = NULL;
}
/**
 * Adds the passed in thread to the correct priority queue
 * ready to be scheduled by the operating system
 *
 * @param thrd Thread to add to the running queues
 */
void add_thread_to_running_queues(thread_t* thrd)
{
    thread_t* list = NULL;
    thread_t** head;

    switch(thrd->priority) 
    {
    case PRIORITY_REALTIME:
        list = realtime_priority_head;
        head = &realtime_priority_head;
        break;
    case PRIORITY_HIGH:
        list = high_priority_head;
        head = &high_priority_head;
        break;
    case PRIORITY_NORMAL:
        list = normal_priority_head;
        head = &normal_priority_head;
        break;
    case PRIORITY_LOW:
        list = low_priority_head;
        head = &low_priority_head;
        break;
    default:
        //Error don't do anything
        return;
        break;
    }
    
    if (list != NULL)
    {
        while(list->next != NULL)
        {
            list = list->next;
        }

        //list now points to the last thread in the desired running queue
        list->next = thrd;
        thrd->prev = list;
        thrd->next = NULL;
    }
    else
    {
        //Head is empty, set it to this thread
        (*head) = thrd;
        thrd->prev = NULL;
        thrd->next = NULL;
    }
    //It is now attached
}
/**
 * This function is called by default when a thread finishes. 
 *
 * More specifically when a thread exits its initial function this
 * function ends up being called. It's ultimate goal is to remove
 * the current thread from the active running queues and destroy
 * itself. Currently it removes itself but then goes in an infinite
 * loop, however, at the next kernel tick another thread will be 
 * scheduled and this thread then is effectivly gone, however, it's
 * memory is not freed. Obviously the freeing part needs to be
 * implemented
 *
 * @return SUCCESS
 */
int sys_thread_exit()
{
    remove_current_thread_from_running_queues();

    //OK, so now current_thread is not in any running queues
    
    //Lets switch the current thread to be the null task, 
    //set our current stack to a junk stack, (like the old original kernel stack)
    //and then schedule
    asm volatile ("lss         kernel_stack_desc, %esp");

    //THIS IS A HACK
    //We will copy our current kernel stack from the dying task to our jumk task
    memcpy(kernel_stack, current_thread->kernel_stack, (PAGE_SIZE >> 2));

    //Clear up memory
    k_free(current_thread->parent_process);
    k_free(current_thread);

    //Probably set current_thread to the null thread then perform a task switch
    return SUCCESS;
}
/**
 * Looks through the list of any sleeping tasks and see if any are
 * ready to be woken up
 *
 * @param ticks The current tick count
 */
void check_sleeping_tasks(ulong ticks)
{
    //Iterate through tasks
    thread_t* temp;

    //Only check if there are sleeping tasks
    if (sleep_head == NULL)
        return;

    temp = sleep_head;

    while (temp != NULL) 
    {
        if (temp->wake_up_time <= ticks)
        {
            //Wake this thread up
            //Remove it from the sleep list
            if (temp->snext != NULL)
            {
                sleep_head = temp->snext;
                temp->snext->sprev = NULL;
                klprintf(7, "&Wake up Thread 0x%08x", temp);
                add_thread_to_running_queues(temp);
                temp->parent_process->state = TASK_READY;
                thread_t* tt = temp;
                temp = temp->snext;
                tt->snext = NULL;
                tt->sprev = NULL;
            }
            else
            {
                sleep_head = NULL;
                add_thread_to_running_queues(temp);
                klprintf(7, "Wake up Thread 0x%08x", temp);
                temp->parent_process->state = TASK_READY;
                temp->snext = NULL;
                temp->sprev = NULL;
                break;
            }
        }
        else
        {
            //Don't keep checking as they are stored in order
            klprintf(7, "None to Wake");
            break;
        }
    }

}
/**
 * Puts the current thread to sleep for the specified number of
 * milliseconds
 *
 * @param milliseconds Time for this thread to sleep for
 * @return SUCCESS
 */
int sys_msleep(uint milliseconds)
{
    char interrupts_enabled;
    interrupts_enabled = return_interrupt_status();
    disable();

    //Add our task to the sleep list
    thread_t* current = get_current_thread();
    thread_t* t;

    current->wake_up_time = get_system_ticks() + (ulong)(milliseconds /(1000 / TIMER_FRQ));

    if (sleep_head == NULL)
    {
        sleep_head = current_thread;
        current_thread->snext = NULL;
        current_thread->sprev = NULL;
    }
    else
    {
        t = sleep_head;

        while(current->wake_up_time >= t->wake_up_time) 
        {
            if (t->snext == NULL)
            {
                //At end of queue
                break;
            }
            t = t->snext;
        }

        if (current->wake_up_time >= t->wake_up_time)
        {
            //We are at end of queue add to end
            t->snext = current;
            current->sprev = t;
            current->snext = NULL;
        }
        else
        {
            //Insert here
            thread_t* temp;
            temp = t->sprev->snext;
            t->sprev->snext = current;
            current->sprev = t;
            temp->sprev = current;
            current->snext = temp;
        }
    }
    
    //remove current from its queue
    remove_current_thread_from_running_queues();

    //Before we shedule we need to set currrent thread to blocked for the scheduler
    current->parent_process->state = TASK_STOPPED;

    //Then call schedule
    schedule();

    //Once we're here It should mean we have slept


    if (interrupts_enabled)
        enable();

    return SUCCESS;
}
/** 
 * Returns a pointer to the idle process
 *
 * @return ptr to the idle process
 */
process_t* get_idle_task()
{
    return process_head;
};
/**
 * Returns the next successive Process ID
 *
 * @return Next successive process id
 */
ulong get_pid()
{
    return current_pid++;
}
/**
 * Returns the next successive Thread ID
 * 
 * @return Next successive thread ID
 */
ulong get_tid()
{
    return current_tid++;
}
/**
 * Returns the process name of the currently executing
 * thread
 *
 * @return Name of the currently executing process
 */
char* get_current_task_name()
{
    return current_process->name;
}
/**
 * Returns a pointer to the current process
 *
 * @return Ptr to the current process
 */
process_t* get_current_task()
{
    return current_process;
}
/**
 * Returns a pointer to the current thread
 * 
 * @return Ptr to the current thread
 */
thread_t* get_current_thread()
{
    return current_thread;
}

