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

void test_function(void* ptr)
{
    static uint my_loops = 0;
    ptr = ptr;

    for (;;)
    {
        klprintf(10, "In %s: %d", get_current_task_name(), my_loops++);
    }
        //asm("hlt");
}
