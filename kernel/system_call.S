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
 * Compiler: gcc
 *
 * Author: Colin Goudie
 **************************************************************************/
.align 2

.global timer_interrupt

timer_interrupt:
    push    %ds          ;Save ds, es and fs
    push    %es
    push    %fs


