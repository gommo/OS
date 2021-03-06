/*************************************************************************
 *                           -=-=-= boot.S =-=-=-
 *
 * THIS FILE WAS CREATED AND IS FOR ELEC4710 - REALTIME SYSTEMS
 * PROJECT ASSIGNMENT
 *
 * This file contains initialisation code which is advisable to do before
 * we jump into our C code. These things involve setting up a stack, 
 * Setting up a minimal GDT and blank IDT.
 *
 * Assembler: Gas
 *
 * Author: Colin Goudie
 *
 *************************************************************************/
.text
.align 4

.global init, idt, gdt, tss_desc
.extern k_main, start_stack

/* In this function we are going to set up the basic functionality      */
/* before we move to C code. This includes setting up area to put our   */
/* Interrupt Descriptor Table and Global Descriptor tables, as well as  */
/* a stack to use.                                                      */

init:
    
    call        setup_idt                       # Sets up our IDT
    call        setup_gdt                       # Sets up our GDT
    movl        $0x10, %eax                     # Reload all segment registers
    mov         %ax, %ds
    mov         %ax, %es
    mov         %ax, %fs
    mov         %ax, %gs
/* Sets up our stack register and stack segment to valid addresses. The */
/* stack is defined in an area that is declared in kernel.c             */
    lss         kernel_stack_desc, %esp
    
    /* Reset EFLAGS */
    push        $2
    popf
    
    pushl       %ebx                            # Push the pointer to our multiboot header info structure
    
    call k_main                                 # Jump to our C code

    addl        $4, %esp                        # Clean up the stack

    cli
    hlt

.long   0
idt_descr:
    .word   256*8-1                             # IDT has 256 entries
    .long   idt                                 # Ptr to our idt 

.long   0
idt:
    .fill   256, 8, 0                           # Black IDT Table with     
                                                # 256 Entries         

/* This section sets up the IDT. This basically will set up 256 entries
   that will simply print something out on the screen. The actual entries
   will be replaced with valid ones once we go to our C code    */
setup_idt:
    lidt    idt_descr                           # Loads our IDT
    ret

/* This section simply sets the GDT to the one hardcoded below */
setup_gdt:
    lgdt    gdt_descr                           # Loads our GDT
    ret

.long   0
/* GDT_Table Descriptor. This structure is in the format required by the
   lgdt assembly instruction. See the intel dev Manual 2 for more details */
gdt_descr:
    .word   256*8-1                             # GDT has 256 entries
    .long   gdt

.long   0
/* Out Glocal Descriptor Table */
gdt:
    .quad   0x0000000000000000  /* The first entry is blank as required */

    .quad   0x00c09a0000001000  /* This entry is a 16MB Execute/Read 
                                   Code Segment. S=1, Privilege = 0,
                                   G=1, D/B=1, AVL = 0              */

    .quad   0x00c0920000001000  /* This entry is 16MB Read/Write 
                                   Data Segment. S=1, Privilege = 0,
                                   G=1, D/B=1, AVL = 0              */
tss_desc:
    .fill   253, 8, 0           /* Fill the rest with zeros */
    
