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

void spawned_task_function(void* ptr);
void realtime_task(void* ptr);

void test_function(void* ptr)
{
    uint my_loops = 0;
    ptr = ptr;

    for (;;)
    {
        sema_wait(sema);
        klprintf(10, "In %s: %d", get_current_task_name(), my_loops++);
        msleep(500);
        create_process("Realtime Task", &realtime_task, NULL, PRIORITY_REALTIME);
        sema_signal(sema);
    }
}

void test_function2(void* ptr)
{
    uint my_loops = 0;
    ptr = ptr;

    create_process("Spawned Task", &spawned_task_function, NULL, PRIORITY_HIGH);
   
    for (;;)
    {
        sema_wait(sema);
        klprintf(11, "In %s: %d", get_current_task_name(), my_loops++);
        msleep(300);
        sema_signal(sema);
    }
}

void spawned_task_function(void* ptr)
{
    uint my_loops = 0;
    ptr = ptr;

    for (;;)
    {
        klprintf(12, "In %s: %d", get_current_task_name(), my_loops++);
        //msleep(100);
        if (my_loops > 15000)
            break;
    }
}

void realtime_task(void* ptr)
{
    uint my_loops = 0;
    ptr = ptr;

    for (;;)
    {
        klprintf(13, "in %s: %d", get_current_task_name(), my_loops++);
        if (my_loops > 500)
            break;
    }
}

