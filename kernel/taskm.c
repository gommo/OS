/*************************************************************************
*                          -=-=-= taskm.c =-=-=-
*
* THIS FILE WAS CREATED FOR ELEC4710 - REALTIME SYSTEMS
* MAJOR PROJECT ASSIGNMENT
*
* This file contains structures and definitons related to task
* Management. This includes the Task State Structure.
*
* Compiler: gcc
*
* Author: Colin Goudie
**************************************************************************/
#include <os/taskm.h>

extern struct tss_descriptor* tss_desc;

void init_taskm(void)
{
    //We have to set up a Task Descriptor in the GDT
    tss_desc->base_address_15_00 = 0xFFFF;
    tss_desc->base_address_23_16 = 0xFF;
    tss_desc->base_address_31_42 = 0xFF;

    klprintf(18, "Wrote to tss_desc");
}
