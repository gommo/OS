/*************************************************************************
 *                            -=-=-= kernel.c =-=-=-
 *
 * THIS FILE WAS CREATED FOR ELEC4710 - REALTIME SYSTEMS
 * MAJOR PROJECT ASSIGNMENT
 *
 * This file contains the kernel main function that is used to initialise
 * the realtime kernel/Operating System 
 *
 * Compiler: gcc
 *
 * Author: Colin Goudie
 **************************************************************************/
#include <os/multiboot.h>
#include <os/kernel.h>
#include <os/platform.h>
#include <os/mm/paging.h>
#include <os/mm/mm.h>
#include <os/idt.h>
#include <os/pic.h>
#include <os/timer.h>
#include <os/taskm/sched.h>
#include <os/taskm/sema.h>
#include <console.h>
#include <stdarg.h>
#include <stdio.h>

extern void test_function(void* ptr);
extern void test_function2(void* ptr);

/* Number of System Ticks */
static ulong system_ticks = 0;
/* The cached state of the interrupts. This starts of as FALSE
   as that is the state of the interrupts when we enter our kernel */
static uchar interrupts_enabled = FALSE;
/** This is the location of the kernel's stack */
long    kernel_stack [ PAGE_SIZE >> 2 ];
/** Initialise a Stack Descriptor pointing at the top of the user_stack
(PAGE>>2)  and pointing to our data segment (0x10) */
struct stack kernel_stack_desc = { &kernel_stack[PAGE_SIZE >> 2], KERNEL_DATA };
/**
 * This will be the global tss structure
 */
struct tss global_tss;
/**
 * This is our gdt (created in init.S)
 */
extern desc_table gdt;

/** Test global semaphore */
sema_handle sema;

/** 
 * This is our first C function that is called. It initialises our kernel.
 * Execution begins here
 *
 * @param info Some bootup parameters and information passed on by grub
 */
int k_main(multiboot_info_t* info) // like main in a normal C program
{
    /* Declare a process that we'll use for the idle process                */
    struct process* idle;
    /* Clear the terminal screen                                            */
	k_clear_screen();
    /* Display a message so we know we are booting ok.                      */
    /* Show us how much memory this machine has                             */
    k_printf("booting...", 0);
    klprintf(2, "Low Memory: %d", info->mem_lower);
    klprintf(3, "High Memory: %d", info->mem_upper);

    /* Initialise the memory manager                                        */
    init_mm();
    /* Initialise our Interrupt Descriptor table to sensible values and     */
    /* Interrupts                                                           */
    init_idt();
    /* Initialise the scheduler                                             */
    init_sched();

    /* Reprogram the Programmable Interrupt Controller                      */
    reprogram_pic( 0x20, 0x28 );
    /* Initialise our Timer for preemptive multitasking                     */
    init_timer();
    /* Enable the IRQ that the Timer is attached to so we will get int's    */
    enable_irq( 0 );

    klprintf(19, "Starting Kernel");

    /* Enabled Interrupts                                                   */
    enable();

    //Add new new process for test
    sema_create(&sema, 1);
    void* temp;
    temp = create_process("Test Proc", &test_function, NULL, PRIORITY_NORMAL);
    klprintf(17, "Test Proc lives @ 0x%08x", temp);
    temp = create_process("Another Proc", &test_function2, NULL, PRIORITY_NORMAL);
    klprintf(18, "Another lives @ 0x%08x", temp);
   
    idle = get_idle_task();

    /* Now we jump to our ring 3 idle task                                  */
    jump_to_ring3_task(idle->thread_list->task_state.ss, 
                      idle->thread_list->task_state.esp, 
                      idle->thread_list->task_state.cs, 
                      idle->thread_list->task_state.eip);
    
    /* We never get here */
    for (;;)
        asm("hlt");

    return 0;    
}

void k_clear_screen() // clear the entire text screen
{
	char *vidmem = (char *) VIDEO_MEMORY;
	unsigned int i=0;
	while(i < (CONSOLE_WIDTH * CONSOLE_HEIGHT * CONSOLE_DEPTH))
	{
		vidmem[i]=' ';
		i++;
		vidmem[i]=WHITE;
		i++;
	};
};

void klprintf(uint line, uchar* fmt, ...)
{
    //Set aside a large buffer for input
    uchar buffer[1024];

    va_list arguments;
    va_start(arguments, fmt);

    vsprintf( buffer, fmt, arguments );

    k_printf(buffer, line);

    va_end(arguments);
}

unsigned int k_printf(char *message, unsigned int line) // the message and then the line #
{
	char *vidmem = (char *) VIDEO_MEMORY;
	unsigned int i=0;

	i=(line * CONSOLE_WIDTH * CONSOLE_DEPTH);

	while(*message!=0)
	{
		if(*message=='\n') // check for a new line
		{
			line++;
			i=(line * CONSOLE_WIDTH * CONSOLE_DEPTH);
			*message++;
		} else {
			vidmem[i]=*message;
			*message++;
			i++;
			vidmem[i]=WHITE;
			i++;
		}
	}

	return(1);
}
/**
* Returns the number of timer ticks the kernel has done 
*
* @return ulong number of ticks
*/
ulong get_system_ticks()
{
    return system_ticks;
}
/**
* Increments the number of kernel ticks by one
*
* @return Returns the new tick count
*/
ulong inc_system_ticks()
{
    return ++system_ticks;
}
/**
* Disables interrupts
*/
void disable()
{
    asm volatile ("cli");
    interrupts_enabled = FALSE;
}
/**
* Enables Interrupts
*/
void enable()
{
    asm volatile ("sti");
    interrupts_enabled = TRUE;
}
/**
* Returns the status of interrupts
*
* @return State of the interrupt flag
*/
uchar return_interrupt_status()
{
    return interrupts_enabled;
}
/**
* Saves the EFlags register by returning a ulong
*
* @return ulong contents of the EFlags register
*/
ulong save_flags()
{
    ulong result;
    asm volatile (  "pushfl\n\t"
                    "popl %0"
                    :"=r" (result) :: "memory");
    return result;
}
/**
* Restores the EFlags register
*
* @param ulong new Contents of the EFlags register
*/
void restore_flags(ulong flags)
{
    asm volatile (  "pushl  %0\n\t"
                    "popfl"
                    :: "r" (flags) : "memory");
}

/**
* This creates a segment descriptor and adds it
* to the GDT
*
* @param uint Segment index into the GDT (Not the selector)
* @param uint Base address of the segment
* @param uint Segment Limit
* @param uint Descriptor Type
* @param uint Privilege level of the segment
* @param uint Present flag
* @param uint Granularity of the segment
*/
void create_gdt_segment_descriptor(   uint segment_index,
                                    uint base_address,
                                    uint segment_limit,
                                    uint segment_type,
                                    uint privilege_level,
                                    uint present,
                                    uint granularity )
{
    gdt[segment_index].descripts.seg_descriptor.segment_limit_15_00 = segment_limit & 0x0000FFFF;
    gdt[segment_index].descripts.seg_descriptor.base_address_15_00 = base_address & 0x0000FFFF;
    gdt[segment_index].descripts.seg_descriptor.base_address_23_16 = (base_address & 0x00FF0000) >> 16;
    gdt[segment_index].descripts.seg_descriptor.segment_type = segment_type;
    gdt[segment_index].descripts.seg_descriptor.descriptor_type = 1; //Code OR Data
    gdt[segment_index].descripts.seg_descriptor.dpl = privilege_level;
    gdt[segment_index].descripts.seg_descriptor.present = present;
    gdt[segment_index].descripts.seg_descriptor.segment_limit_19_16 = (segment_limit & 0x00FF0000) >> 16;
    gdt[segment_index].descripts.seg_descriptor.avl = 1;
    gdt[segment_index].descripts.seg_descriptor.default_operation = 1;  //32 bit segment
    gdt[segment_index].descripts.seg_descriptor.zero = 0;
    gdt[segment_index].descripts.seg_descriptor.granularity = granularity;
    gdt[segment_index].descripts.seg_descriptor.base_address_31_24 = (base_address & 0xFF000000) >> 24;
}


