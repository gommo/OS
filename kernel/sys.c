/*************************************************************************
*                             -=-=-= sys.c =-=-=-
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
**************************************************************************/
#include <os/config.h>
#include <os/platform.h>
#include <os/sys.h>
#include <os/taskm/sema.h>

int system_call( struct handler_stack_frame* frame )
{
    int system_call_number;
    char** system_call_params;
    int* system_call_result;

    system_call_result = (int*)frame->ecx;
    system_call_params = (char**)frame->ebx;
    system_call_number = frame->eax;

    switch( system_call_number )
    {
    case SYS_SEMA_CREATE:
        *system_call_result = semaphore_create( (sema_handle*)system_call_params[0], (int)system_call_params[1] );
        break;
    case SYS_SEMA_WAIT:
        *system_call_result = semaphore_wait( (int)system_call_params[0] );
        break;
    case SYS_SEMA_SIGNAL:
        *system_call_result = semaphore_signal( (int)system_call_params[0] );
        break;
    default:
        *system_call_result = FAILURE;
        break;
    }

    return SUCCESS;
}

