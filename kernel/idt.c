/*************************************************************************
*                             -=-=-= idt.c =-=-=-
*
* THIS FILE WAS CREATED FOR ELEC4710 - REALTIME SYSTEMS
* MAJOR PROJECT ASSIGNMENT
*
* This file contains the function definitions for those declared in idt.h.
* This files main task is to setup the interrupts and their handlers.
*
* Compiler: gcc
*
* Author: Colin Goudie
**************************************************************************/
#include <os/idt.h>
#include <os/kernel.h>
#include <os/timer.h>
#include <os/sys.h>
#include <string.h>

/* defined in system_call.S */
extern void default_interrupt(void);

void kernel_error(int error, long eip, long esp, long eax, long ebx, long ecx, long edx, long edi, long esi);

static BOOL set_interrupt_handler(  GATE_TYPE type,
                                    ushort    interrupt_number,
                                    void*     handler_address,
                                    ushort    segment_selector,
                                    ushort    privilege_level);
/**
* Initialises the Interrupt Descriptor Table to contain valid descriptors.
* These are used when interrupts and exceptions occur and jump to valid
* function handlers
*
* @return BOOL true if 
*/
BOOL init_idt()
{
    int i;

    //First set all our interrupts to the default handler 
    for (i=0; i < NUMBER_OF_INTERRUPTS; i++)
    {
        set_interrupt_handler( INTERRUPT_GATE, i, &default_interrupt, KERNEL_CODE, KERNEL_LEVEL);
    }

    //Set the 19 CPU Specific interrupts
    set_interrupt_handler( INTERRUPT_GATE, 0, &divide_error, KERNEL_CODE, KERNEL_LEVEL);
    set_interrupt_handler( INTERRUPT_GATE, 1, &debug_exception, KERNEL_CODE, KERNEL_LEVEL);
    set_interrupt_handler( INTERRUPT_GATE, 2, &nmi_hardware, KERNEL_CODE, KERNEL_LEVEL);
    set_interrupt_handler( INTERRUPT_GATE, 3, &debug_breakpoint, KERNEL_CODE, KERNEL_LEVEL);
    set_interrupt_handler( INTERRUPT_GATE, 4, &instruction_overflow, KERNEL_CODE, KERNEL_LEVEL);
    set_interrupt_handler( INTERRUPT_GATE, 5, &instruction_overrange, KERNEL_CODE, KERNEL_LEVEL);
    set_interrupt_handler( INTERRUPT_GATE, 6, &invalid_opcode, KERNEL_CODE, KERNEL_LEVEL);
    set_interrupt_handler( INTERRUPT_GATE, 7, &no_coprocessor, KERNEL_CODE, KERNEL_LEVEL);
    set_interrupt_handler( INTERRUPT_GATE, 8, &double_fault, KERNEL_CODE, KERNEL_LEVEL);
    set_interrupt_handler( INTERRUPT_GATE, 9, &coprocessor_segment_overrun, KERNEL_CODE, KERNEL_LEVEL);
    set_interrupt_handler( INTERRUPT_GATE, 10, &invalid_tss, KERNEL_CODE, KERNEL_LEVEL);
    set_interrupt_handler( INTERRUPT_GATE, 11, &segment_not_present, KERNEL_CODE, KERNEL_LEVEL);
    set_interrupt_handler( INTERRUPT_GATE, 12, &stack_fault, KERNEL_CODE, KERNEL_LEVEL);
    set_interrupt_handler( INTERRUPT_GATE, 13, &general_protection_fault, KERNEL_CODE, KERNEL_LEVEL);
    set_interrupt_handler( INTERRUPT_GATE, 14, &page_fault, KERNEL_CODE, KERNEL_LEVEL);
    set_interrupt_handler( INTERRUPT_GATE, 15, &reserved15, KERNEL_CODE, KERNEL_LEVEL);
    set_interrupt_handler( INTERRUPT_GATE, 16, &coprocessor_error, KERNEL_CODE, KERNEL_LEVEL);
    set_interrupt_handler( INTERRUPT_GATE, 17, &alignment_check, KERNEL_CODE, KERNEL_LEVEL);
    set_interrupt_handler( INTERRUPT_GATE, 18, &machine_check, KERNEL_CODE, KERNEL_LEVEL);

    //Set the 16 IRQ Handlers
    /* Timer interrupt */
    set_interrupt_handler( INTERRUPT_GATE, 0x20, &irq0, KERNEL_CODE, KERNEL_LEVEL );
    
    /** Set system call interrupt */
    set_interrupt_handler( INTERRUPT_GATE, 0x80, &system_call_entry, KERNEL_CODE, USER_LEVEL );

    return TRUE;
}

/**
* This function is used to set up an interrupt handler and descriptor. It
* inserts into the IDT a correctly formatted descriptor to handle the
* inputted interrupt number.
*
* @param type               Type of the interrupt descriptor
* @param interrupt_number   Number of the interrupt
* @param handler_address    Address of the handling function
* @param segment_selected   The segment selector specifies the code segment to be
*                           used for the handling function
* @param privilege_level    The privilege level of the handling code
*/
static BOOL set_interrupt_handler(  GATE_TYPE type,
                                    ushort    interrupt_number,
                                    void*     handler_address,
                                    ushort    segment_selector,
                                    ushort    privilege_level)
{
    switch(type)
    {
    case TASK_GATE:
        idt[interrupt_number].descripts.tsk_gate.reserved0 = 0x0;
        idt[interrupt_number].descripts.tsk_gate.tss_segment_selector = segment_selector;
        idt[interrupt_number].descripts.tsk_gate.reserved1 = 0x1;
        idt[interrupt_number].descripts.tsk_gate.type = type;
        idt[interrupt_number].descripts.tsk_gate.dpl = privilege_level;
        idt[interrupt_number].descripts.tsk_gate.present = 0x1;
        idt[interrupt_number].descripts.tsk_gate.reserved3 = 0x0;
        break;
    case INTERRUPT_GATE:
        //klprintf(11, "set_interrupt_handler (INTERRUPT) called %d times", count);    
        idt[interrupt_number].descripts.int_trp_cll_gate.offset_15_00 = (ushort)(0x0000FFFF & (uint)handler_address);
        idt[interrupt_number].descripts.int_trp_cll_gate.segment_selector = segment_selector;
        idt[interrupt_number].descripts.int_trp_cll_gate.param_count = 0x0;
        idt[interrupt_number].descripts.int_trp_cll_gate.zeros = 0x0;
        idt[interrupt_number].descripts.int_trp_cll_gate.type = type;
        idt[interrupt_number].descripts.int_trp_cll_gate.zero2 = 0x0;
        idt[interrupt_number].descripts.int_trp_cll_gate.dpl = privilege_level;
        idt[interrupt_number].descripts.int_trp_cll_gate.present = 0x1;
        idt[interrupt_number].descripts.int_trp_cll_gate.offset_31_16 = (ushort)((uint)handler_address >> 16);
        break;
    case TRAP_GATE:
        idt[interrupt_number].descripts.int_trp_cll_gate.offset_15_00 = (ushort)(0x0000FFFF & (uint)handler_address);
        idt[interrupt_number].descripts.int_trp_cll_gate.segment_selector = segment_selector;
        idt[interrupt_number].descripts.int_trp_cll_gate.param_count = 0x0;
        idt[interrupt_number].descripts.int_trp_cll_gate.zeros = 0x0;
        idt[interrupt_number].descripts.int_trp_cll_gate.type = type;
        idt[interrupt_number].descripts.int_trp_cll_gate.zero2 = 0x0;
        idt[interrupt_number].descripts.int_trp_cll_gate.dpl = privilege_level;
        idt[interrupt_number].descripts.int_trp_cll_gate.present = 0x1;
        idt[interrupt_number].descripts.int_trp_cll_gate.offset_31_16 = (ushort)((uint)handler_address >> 16);
        break;
    case CALL_GATE:
        idt[interrupt_number].descripts.int_trp_cll_gate.offset_15_00 = (ushort)(0x0000FFFF & (uint)handler_address);
        idt[interrupt_number].descripts.int_trp_cll_gate.segment_selector = segment_selector;
        idt[interrupt_number].descripts.int_trp_cll_gate.param_count = 0x0;
        idt[interrupt_number].descripts.int_trp_cll_gate.zeros = 0x0;
        idt[interrupt_number].descripts.int_trp_cll_gate.type = type;
        idt[interrupt_number].descripts.int_trp_cll_gate.zero2 = 0x0;
        idt[interrupt_number].descripts.int_trp_cll_gate.dpl = privilege_level;
        idt[interrupt_number].descripts.int_trp_cll_gate.present = 0x1;
        idt[interrupt_number].descripts.int_trp_cll_gate.offset_31_16 = (ushort)((uint)handler_address >> 16);
        break;
    default:
        return FALSE;
        break;
    }

    return TRUE;
}

/**
* This function handles most interrupts by finding out the interrupt #
* and calling the appropriate handler functions
*
* @param ptr_to_stack Pointer to top of stack
*/
void main_interrupt_handler(uint ptr_to_stack)
{
    struct handler_stack_frame* frame = (struct handler_stack_frame*)&ptr_to_stack;

    if (frame->interrupt_number)
    {
        klprintf(15, "Handling interrupt #%d, ticks=%d", frame->interrupt_number, get_system_ticks());

        switch(frame->interrupt_number)
        {
        
        case DIVIDE_ERROR: case DEBUG_EXCEPTION: case NMI_HARDWARE: case DEBUG_BREAKPOINT:
        case INSTRUCTION_OVERFLOW: case INSTRUCTION_OVERRANGE: case INVALID_OPCODE: case NO_COPROCESSOR:
        case DOUBLE_FAULT: case COPROCESSOR_SEG_OVERRUN: case INVALID_TSS: case SEGMENT_NOT_PRESENT:
        case STACK_FAULT: case GENERAL_PROTECTION_FAULT: case PAGE_FAULT: case RESERVED15: case COPROCESSOR_ERROR:
        case ALIGNMENT_CHECK: case MACHINE_CHECK:
            kernel_error(frame->interrupt_number, frame->stck_frame.eip, frame->stck_frame.esp,
                        frame->eax, frame->ebx, frame->ecx, frame->edx, frame->edi, frame->esi);
            break;
        case TIMER_IRQ:  
            timer_interrupt_handler();
            break;
        case SYSTEM_CALL:
            system_call( frame );
            break;
        }
    }
}


/**
* This will be the default exception handler that simply displays a message
* Replace this later with a good panic function that can display some
* useful information about the error
*/
void default_interrupt_handler()
{
    k_printf("!default_exception_handler called", 23);

    for (;;)
    {

    }
}

/**
 * This is a kernel debug function that is called when a fault interrupt 
 * occurs. It displays the source of the interrupt
 */
void kernel_error(int error, long eip, long esp, long eax, long ebx, long ecx, long edx, long edi, long esi)
{
    char error_msg[80];
    //int error = frame->interrupt_number;

    switch(error)
    {
    case DIVIDE_ERROR:
        strcpy(error_msg, "!!Divide By Zero Exception!!");
    	break;
    case DEBUG_EXCEPTION:
        strcpy(error_msg, "!!Debug Exception!!");
    	break;
    case NMI_HARDWARE:
        strcpy(error_msg, "!!NMI Hardware Exception!!");
        break;
    case DEBUG_BREAKPOINT:
        strcpy(error_msg, "!!Debug Breakpoint!!");
        break;
    case INSTRUCTION_OVERFLOW:
        strcpy(error_msg, "!!Instruction Overflow Exception!!");
        break;
    case INSTRUCTION_OVERRANGE:
        strcpy(error_msg, "!!Instruction Overrange Exception!!");
        break;
    case INVALID_OPCODE:
        strcpy(error_msg, "!!Invalid Opcode Exception!!");
        break;
    case NO_COPROCESSOR:
        strcpy(error_msg, "!!No Coprocessor Exception!!");
        break;
    case DOUBLE_FAULT:
        strcpy(error_msg, "!!Double Fault Exception. Is this Tennis??!!");
        break;
    case COPROCESSOR_SEG_OVERRUN:
        strcpy(error_msg, "!!Coprocessor Segment Overrun Exception!!");
        break;
    case INVALID_TSS:
        strcpy(error_msg, "!!Invalid TSS Exception!!");
        break;
    case SEGMENT_NOT_PRESENT:
        strcpy(error_msg, "!!Segment Not Present Exception!!");
        break;
    case STACK_FAULT:
        strcpy(error_msg, "!!Stack Fault Exception!!");
        break;
    case GENERAL_PROTECTION_FAULT:
        strcpy(error_msg, "!!General Protection Fault!!");
        break;
    case PAGE_FAULT:
        strcpy(error_msg, "!!Page Fault Exception!!");
        break;
    case RESERVED15:
        strcpy(error_msg, "!!Reserved15 Exception!!");
        break;
    case COPROCESSOR_ERROR:
        strcpy(error_msg, "!!Coprocessor Error Exception!!");
        break;
    case ALIGNMENT_CHECK:
        strcpy(error_msg, "!!Alignment Check Exception!!");
        break;
    case MACHINE_CHECK:
        strcpy(error_msg, "!!Machine Check Exception!!");
        break;
    default:
        strcpy(error_msg, "!!Unknown Exception!!");
        break;
    }

    klprintf(20, error_msg);
    klprintf(21, "EIP: 0x%08x ESP: 0x%08x", eip, esp);

    for (;;)
    {

    }

}




