;*************************************************************************
;*                             -=-=-= boot.S =-=-=-
;*
;* THIS FILE WAS CREATED AND IS FOR ELEC4710 - REALTIME SYSTEMS
;* PROJECT ASSIGNMENT
;*
;* This file contains the relocatable start function that resides at 
;* address 0x10000. This is the first piece of code executed and basically
;* calls our C k_main function.
;*
;* To enable our kernel to boot via GRUB this also contains what is called
;* a multiboot header which is a requirement by GRUB.
;*
;* Assembler: NASM
;*
;* Author: Colin Goudie
;*
;* Note: This bootup code was based on code from 
;*       http://www.osdever.net/tutorials.php?cat=0&sort=1
;*************************************************************************/
%include "grub.inc" ; needed for the multiboot header

[BITS 32]

[global start]
[extern k_main] ; this is in the c file

start:
  call k_main

  cli  ; stop interrupts
  hlt ; halt the CPU




;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Multiboot header for GRUB bootloader. This must be in the first 8K
; of the kernel file. We use the aout kludge so it works with ELF,
; DJGPP COFF, Win32 PE, or other formats.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; these are in the linker script file
EXTERN code, bss, end

ALIGN 4
mboot:
	dd MULTIBOOT_HEADER_MAGIC
	dd MULTIBOOT_HEADER_FLAGS
	dd MULTIBOOT_CHECKSUM
; aout kludge. These must be PHYSICAL addresses
	dd mboot
	dd code
	dd bss
	dd end
	dd start
