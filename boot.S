/*************************************************************************
 *                           -=-=-= boot.S =-=-=-
 *
 * THIS FILE WAS CREATED AND IS FOR ELEC4710 - REALTIME SYSTEMS
 * PROJECT ASSIGNMENT
 *
 * This file contains the relocatable start function that resides at
 * address 0x100000. This is the first piece of code executed.
 *
 * To enable our kernel to boot via GRUB this also contains what is called
 * a multiboot header which is a requirement by GRUB.
 *
 * After the initial setup of the multiboot header it calls 'init' which
 * is in our init.S file.
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
#include <os/multiboot.h>
	
	.text

    .extern init
	.globl	start, _start

/* Execution Starts Here. Grub Loads our kernel from location 0x100000  */
/* which is where our linker script link.ld puts this first function    */	

start:
_start:
/* Grub requires the first bytes of our kernel to match a specific      */
/* layout. As a result, below we set this up but we jump over it to our */
/* actual code.                                                         */
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

	/* Jump to our initialisation code in init.S                        */
	call	init
	
