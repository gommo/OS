/*************************************************************************
 *                           -=-=-= boot.S =-=-=-
 *
 * THIS FILE WAS CREATED AND IS FOR ELEC4710 - REALTIME SYSTEMS
 * PROJECT ASSIGNMENT
 *
 * This file contains the relocatable start function that resides at
 * address 0x100000. This is the first piece of code executed and sets
 * up the initial space for the IDT, sets up a simple GDT and calls our
 * first C function k_main
 *
 * To enable our kernel to boot via GRUB this also contains what is called
 * a multiboot header which is a requirement by GRUB.
 *
 * Assembler: Gas
 *
 * Author: Colin Goudie
 *
 * Note: A lot of the initial code for this module comes from GRUB. GRUB is
 * our GPL bootloader and this file along with multiboot.h is used to enable
 * our kernel to be able to be booted from GRUB.
 *
 *************************************************************************/
/* boot.S - bootstrap the kernel */
/* Copyright (C) 1999, 2001  Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.
 
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
 
   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.  */

#define ASM	1
#include <multiboot.h>
	
	.text

    .extern k_main
	.globl	start, _start
start:
_start:
	jmp	multiboot_entry

	/* Align 32 bits boundary.  */
	.align	4
	
	/* Multiboot header.  */
multiboot_header:
	/* magic */
	.long	MULTIBOOT_HEADER_MAGIC
	/* flags */
	.long	MULTIBOOT_HEADER_FLAGS
	/* checksum */
	.long	-(MULTIBOOT_HEADER_MAGIC + MULTIBOOT_HEADER_FLAGS)
#ifndef __ELF__
	/* header_addr */
	.long	multiboot_header
	/* load_addr */
	.long	_start
	/* load_end_addr */
	.long	_edata
	/* bss_end_addr */
	.long	_end
	/* entry_addr */
	.long	multiboot_entry
#endif /* ! __ELF__ */

multiboot_entry:
	/* Initialize the stack pointer.  */
    #call    setup_idt                           # Setup the IDT
	movl	$(stack + STACK_SIZE), %esp

	/* Reset EFLAGS.  */
	pushl	$0
	popf

	//* Push the pointer to the Multiboot information structure.  */
	//pushl	%ebx
	///* Push the magic value.  */
	//pushl	%eax

	/* Now enter the C main function...  */
	call	k_main

    cli
    hlt

idt_descr:
    .word 256*8-1               # IDT has 256 entries
    .long idt

idt:
    .fill 256, 8, 0             # Blank IDT Table with 256 entries

    

	/* Our stack area.  */
	.comm	stack, STACK_SIZE
	
