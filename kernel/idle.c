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

void idle_task(void* ptr)
{
    ptr = ptr;
    klprintf(9, "In IdleTask");

    for (;;){}
        //asm volatile ("hlt");
}
