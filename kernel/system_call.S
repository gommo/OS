/*************************************************************************
 *                        -=-=-= system_call.S =-=-=-
 *
 * THIS FILE WAS CREATED FOR ELEC4710 - REALTIME SYSTEMS
 * MAJOR PROJECT ASSIGNMENT
 *
 * This file contains the assembly functions that make up the system calls
 * of this kernel. System calls are used by user programs, exception and
 * interrupts to transfer control to the kernel to allow action to take 
 * place. 
 *
 * Compiler: Gas
 *
 * Author: Colin Goudie
 **************************************************************************/
.align 2

.global default_interrupt
.extern default_interrupt_handler
.extern main_interrupt_handler

default_interrupt:
    pushl       %ebp        /* Packup the current registers */
    pushl       %eax
    pushw       %ds
    pushw       %es
    pushl       %ebx
    pushl		%ecx
    pushl       %edx
    pushl       %edi
    pushl       %esi
        
    movl        $0x10, %eax        /* Move our level0 datasegment into our
                                      segment registers */
                                      
    movw        %ax, %ds
    movw        %ax, %es
    
    call        default_interrupt_handler
    
	popl	%esi
	popl	%edi
	popl	%edx
	popl	%ecx
	popl	%ebx

	popw	%es
	popw	%ds
	popl	%eax

	popl	%ebp
    
    iret

/**
 * This is the main interrupt assembly code
 * that calls our C handler code
 */
main_interrupt:
	// Push all general purpose instructions in this order
	// EAX, ECX, EDX, EBX, ESP, EBP, ESI, EDI
	pushal
	
	// Push our segment registers
	pushl	%gs
	pushl	%fs
	pushl	%es
	pushl	%ds
	
	movl        $0x10, %eax        /* Move our level0 datasegment into our
                                      segment registers */
                                      
    movw        %ax, %ds
    movw        %ax, %es
    movw        %ax, %fs
    movw        %ax, %gs
    
	// Jump to our C code handler
	call	main_interrupt_handler
	
	// Pop our segment registers back off
	popl	%ds
	popl	%es
	popl	%fs
	popl	%gs
		
	//Pop our general registers off
	popal
	
	//Restore stack because of our previous pushing of interrupt numbers
	addl	$8, %esp 
	//Return from interrupt
	iret

/**
 * This macro is an excellent idea taken from Tabos OS to generate interrupt
 * handling functions. The BUILD_INTERRUPT macro includes an error code and
 * is used for CPU Exceptions that don't include an error code by default. It
 * is also used to build IRQ handlers. 
 */
#define BUILD_INTERRUPT(name, number)\
.global name; \
name: \
	pushl	$0; \
	pushl	$number;   \
	jmp		main_interrupt;

/**
 * Builds an interrupt handler but expects that the CPU pushes an error
 * code onto the stack. The interrupts that do that can be found in the
 * Intel Dev Manual #3 S:5.3 Table 5-1
 */
#define BUILD_INTER_WEC(name, number)\
.global name; \
name: \
	pushl	$number;   \
	jmp		main_interrupt;
	
/**
 * Build the 18 intel defined exceptions
 */
BUILD_INTERRUPT( divide_error, 0 );
BUILD_INTERRUPT( debug_exception, 1);
BUILD_INTERRUPT( nmi_hardware, 2);
BUILD_INTERRUPT( debug_breakpoint, 3);
BUILD_INTERRUPT( instruction_overflow, 4);
BUILD_INTERRUPT( instruction_overrange, 5);
BUILD_INTERRUPT( invalid_opcode, 6);
BUILD_INTERRUPT( no_coprocessor, 7);
BUILD_INTER_WEC( double_fault, 8);
BUILD_INTERRUPT( coprocessor_segment_overrun, 9);
BUILD_INTER_WEC( invalid_tss, 10);
BUILD_INTER_WEC( segment_not_present, 11);
BUILD_INTER_WEC( stack_fault, 12);
BUILD_INTER_WEC( general_protection_fault, 13);
BUILD_INTER_WEC( page_fault, 14);
BUILD_INTERRUPT( reserved15, 15);
BUILD_INTERRUPT( coprocessor_error, 16);
BUILD_INTER_WEC( alignment_check, 17);
BUILD_INTERRUPT( machine_check, 18);
/**
 * Build the 19-31 reserved exceptions 
 */
 BUILD_INTERRUPT( reserved19, 19);
 BUILD_INTERRUPT( reserved20, 20);
 BUILD_INTERRUPT( reserved21, 21);
 BUILD_INTERRUPT( reserved22, 22);
 BUILD_INTERRUPT( reserved23, 23);
 BUILD_INTERRUPT( reserved24, 24);
 BUILD_INTERRUPT( reserved25, 25);
 BUILD_INTERRUPT( reserved26, 26);
 BUILD_INTERRUPT( reserved27, 27);
 BUILD_INTERRUPT( reserved28, 28);
 BUILD_INTERRUPT( reserved29, 29);
 BUILD_INTERRUPT( reserved30, 30);
 BUILD_INTERRUPT( reserved31, 31);
 /**
  * Build our 16 IRQ interrupts generated from
  * hardware attached to our PIC
  */
 BUILD_INTERRUPT( irq0, 0x20);
 BUILD_INTERRUPT( irq1, 0x21);
 BUILD_INTERRUPT( irq2, 0x22);
 BUILD_INTERRUPT( irq3, 0x23);
 BUILD_INTERRUPT( irq4, 0x24);
 BUILD_INTERRUPT( irq6, 0x25);
 BUILD_INTERRUPT( irq7, 0x26);
 BUILD_INTERRUPT( irq8, 0x27);
 BUILD_INTERRUPT( irq9, 0x28);
 BUILD_INTERRUPT( irq10, 0x29);
 BUILD_INTERRUPT( irq11, 0x2A);
 BUILD_INTERRUPT( irq12, 0x2B);
 BUILD_INTERRUPT( irq13, 0x2C);
 BUILD_INTERRUPT( irq14, 0x2D);
 BUILD_INTERRUPT( irq15, 0x2E);
 
   