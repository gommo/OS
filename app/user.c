/*************************************************************************
*                            -=-=-= user.c =-=-=-
*
* THIS FILE WAS CREATED FOR ELEC4710 - REALTIME SYSTEMS
* MAJOR PROJECT ASSIGNMENT
*
* This file contains the user code
*
* Compiler: gcc
*
* Author: Colin Goudie
**************************************************************************/
#include <os/config.h>
#include <os/kernel.h>
#include <os/taskm/sched.h>
#include <os/os.h>

extern sema_handle sema;

void test_function(void* ptr)
{
    struct process* current;
    uint my_loops = 0;
    ptr = ptr;

    current = get_current_task();

    klprintf(12, "SS:0x%02x ESP:0x%08x SS0:0x%02x ESP0:0x%08x", current->thread_list->task_state.ss,
        current->thread_list->task_state.esp,
        current->thread_list->task_state.ss0,
        current->thread_list->task_state.esp0);

    
    for (;;)
    {
        klprintf(11, "In %s: %d", get_current_task_name(), my_loops++);
        sleep(1);
    }
}

void test_function2(void* ptr)
{
    struct process* current;
    uint my_loops = 0;
    ptr = ptr;

    current = get_current_task();

    klprintf(14, "SS:0x%02x ESP:0x%08x SS0:0x%02x ESP0:0x%08x", current->thread_list->task_state.ss,
        current->thread_list->task_state.esp,
        current->thread_list->task_state.ss0,
        current->thread_list->task_state.esp0);
    
    
    for (;;)
    {
        klprintf(13, "In %s: %d", get_current_task_name(), my_loops++);
        sleep(1);
    }
}
