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
#include <os/sched.h>
#include <os/kernel.h>

/** TSS Descriptor created in init.S */
extern struct tss_descriptor tss_desc;
/** Global TSS Structure created in kernel.c */
extern struct tss global_tss;

void init_taskm(void)
{
    uint temp;
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

    asm volatile ("movl %%esp, %%eax\n\t" : "=r" (temp) :: "memory");
    global_tss.ss0 = KERNEL_DATA;
    global_tss.esp0 = temp;

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


}
