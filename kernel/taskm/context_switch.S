/*************************************************************************
 *                           -=-=-= context_switch.S =-=-=-
 *
 * THIS FILE WAS CREATED AND IS FOR ELEC4710 - REALTIME SYSTEMS
 * PROJECT ASSIGNMENT
 *
 * This file contains the assembly code neccessary to perform a context 
 * switch between two threads
 *
 * Assembler: Gas
 *
 * Author: Colin Goudie
 *
 *************************************************************************/
 /**
  * void context_switch( unsigned long* old_task_state, unsigned long* new_task_state, uchar handled_new );
  */
.global context_switch
 
context_switch:
    
    pushal                      /* Push all general purpose registers onto stack                    */
    
    /* Check to see if we need to issue a leave command */
    cmp     $0, 44(%esp)
    
    movl    36(%esp), %eax      /* Put the address of old_task_state in eax                         */
    movl    %esp, 4(%eax)       /* move the current stack pointer into esp0 in task_state structure */
    movw    %ss, 8(%eax)        /* move the current ss into ss0 in old_task_state structure         */
    
    movl    40(%esp), %eax      /* Put the address of new_task_state in eax                         */
    movl    4(%eax), %esp       /* Move the new esp0 into esp                                       */
    movw    8(%eax), %ss        /* Move the new ss0 into ss                                         */
    
    popal
 
    jz      here
    leave
here:    
    ret