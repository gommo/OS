/*************************************************************************
*                            -=-=-= os.c =-=-=-
*
* THIS FILE WAS CREATED FOR ELEC4710 - REALTIME SYSTEMS
* MAJOR PROJECT ASSIGNMENT
*
* This file contains functions that the operating system will issue system
* calls for. It is used by user applications to access functions that require
* kernel intervention.
*
* Compiler: gcc
*
* Author: Colin Goudie
**************************************************************************/
#include <os/config.h>
#include <os/sys.h>
#include <os/taskm/sema.h>
#include <os/os.h>

int sema_create(sema_handle* sem_handle, int value)
{
    int result;
    SYSTEM_CALL_FUNC( SYS_SEMA_CREATE, &sem_handle, &result );
    return result;
}

int sema_wait(sema_handle sem_handle)
{
    int result;
    SYSTEM_CALL_FUNC( SYS_SEMA_WAIT, &sem_handle, &result);
    return result;
}

int sema_signal(sema_handle sem_handle)
{
    int result;
    SYSTEM_CALL_FUNC( SYS_SEMA_SIGNAL, &sem_handle, &result);
    return result;
}

int thread_exit() 
{
    int result;
    SYSTEM_CALL_FUNC( SYS_THREAD_EXIT, 0, &result);
    return result;
}

