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

default_interrupt:
    pushl       %ebp        /* Packup the current registers */
    pushl       %eax
    pushw       %ds
    pushw       %es
    pushl       %ebx
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
    

