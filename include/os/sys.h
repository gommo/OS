/*************************************************************************
*                             -=-=-= sys.h =-=-=-
*
* THIS FILE WAS CREATED FOR ELEC4710 - REALTIME SYSTEMS
* MAJOR PROJECT ASSIGNMENT
*
* This file contains the function that are used to do system calls. System
* calls are called from the result of user code using functions that require
* kernel actions.
*
* Compiler: gcc
*
* Author: Colin Goudie
*************************************************************************/
#ifndef __SYS_H__
#define __SYS_H__

#include <os/platform.h>

enum
{
    SYS_SEMA_CREATE,
    SYS_SEMA_WAIT,
    SYS_SEMA_SIGNAL,
    SYS_SEMA_DESTROY,
    SYS_THREAD_EXIT,
    SYS_SLEEP
} SYSTEM_CALL_ENUM;
/**
 * This MACRO was taken from tabOS. It basically is a good way of issuing
 * system calls, allowing you to pass parameters and get return values.
 * Basically a user function, defined outside the kernel is a wrapper for
 * this macro. This macro then ends up expanding to
 *  Placing the system call number in eax
 *  Placing the ptr to the first parameter in ebx
 *  Expecting the return value in ecx
 *  Issues int 0x80 which enters the kernel
 *
 *  The kernel then looks @ the system call number and calls the appropriate
 *  kernel function to handle this request
 */
#define SYSTEM_CALL_FUNC(number, params, ret_value) \
    asm volatile( "int $0x80" :: "a"(number), "b"(params), "c"(ret_value));


/**
 * System call definition.
 *
 * @param handler_stack_frame* Ptr to our stack frame
 */
int system_call( struct handler_stack_frame* frame );

#endif

