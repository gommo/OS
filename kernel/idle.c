/*************************************************************************
*                             -=-=-= idle.c =-=-=-
*
* THIS FILE WAS CREATED FOR ELEC4710 - REALTIME SYSTEMS
* MAJOR PROJECT ASSIGNMENT
*
* This filesimply contains the kernel's idle task which does nothing
*
* Compiler: gcc
*
* Author: Colin Goudie
**************************************************************************/
#include <os/kernel.h>
#include <os/taskm/sched.h>

/**
 * This is the idle task that runs when no other tasks are ready to run
 *
 * @param ptr Parameters passed to this task
 */
void idle_task(void* ptr)
{
    struct process* current;
    static uint my_loops = 0;
    ptr = ptr;

    current = get_current_task();

    klprintf(9, "SS:0x%02x ESP:0x%08x SS0:0x%02x ESP0:0x%08x", current->thread_list->task_state.ss,
                                                                current->thread_list->task_state.esp,
                                                                current->thread_list->task_state.ss0,
                                                                current->thread_list->task_state.esp0);
    
    for (;;)
    {
        klprintf(8, "In %s: %d", get_current_task_name(), my_loops++);
    }
        
}
