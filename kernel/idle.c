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

void idle_task(void* ptr)
{
    static uint my_loops = 0;
    ptr = ptr;
    
    for (;;)
    {
        klprintf(9, "In %s: %d", get_current_task_name(), my_loops++);
    }
        
}
