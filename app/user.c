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

void test_function()
{
    klprintf(9, "In usermode");

    for (;;){}
        //asm("hlt");
}
