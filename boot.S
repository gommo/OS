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

BITS 32

GLOBAL start, gdt, idt
EXTERN k_main               ; this is in the c file

start:
    jmp init                ; jump to our init code
    
start2:
    call k_main

    cli                     ; stop interrupts
    hlt                     ; halt the CPU

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Multiboot header for GRUB bootloader. This must be in the first 8K
; of the kernel file. We use the aout kludge so it works with ELF,
; DJGPP COFF, Win32 PE, or other formats.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

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

;In our init code we want to set up our Global Descriptor Table,
;our Interrupt Descriptor Table and our Stack
init:
    call setup_idt                  ; Setup the IDT 
    call setup_gdt                  ; Setup the GDT
    
    ;Now we reset out segment registers
    mov     eax, 0x10     ;This will be used as a segment selector
                          ;The 0x10 equates to segment#2 because
                          ;Bit 1-0 are both 0 for Privilege Level 0
                          ;Bit 2 = GDT, Bit 3..15 is the segment #
                          ;So bit 3 = 0, bit 4 = 1 which means 0x10
                          ;This selects our data segment in our GDT
    

    jmp start2                      ; Jump to the code to go to our C code

;This section sets the up IDT Table. This basically will set up 256
;entries that will simply print something out on the screen. The actual
;entires will be replaced with valid ones once we go to our C code
setup_idt:
    lidt    [idt_descr]
    ret

;This section simply sets the GDT Table to the one hard coded below
setup_gdt:
    lgdt    [gdt_descr]
    ret

;IDT_Table descriptor. This structure is in a format required by the 
; lidt assembly instruction
idt_descr:
    dw 256*8-1                  ; IDT has 256 entries
    dd idt

;Our IDT (Interrupt Descriptor Table)
idt:
    times 512 dd 0x00000000     ; Fill with Null entries initially

;GDT_Table descriptor. This structure is in a format required by the
; lgdt assembly instruction
gdt_descr:
    dw 256*8-1                  ; GDT has 256 entries
    dd gdt

;Our GDT (Global Descriptor Table)
gdt:
    dd 0x00000000   ; First entry is blank
    dd 0x00000000   ; As required

    dd 0x00001000   ; This Entry is 16MB Execute/Read Code Segment
    dd 0x00c09a00   ; S=1, Privilege=0, G=1, D/B=1, AVL=0

    dd 0x00001000   ; This Entry is 16MB Read/Write Data Segment
    dd 0x00c09200   ; S=1, Privilege=0, G=1, D/B=1, AVL=0

    dd 0x00000000
    dd 0x00000000

    times 504 dd 0x00000000 ; Fill the rest of the GDT with zero entries

