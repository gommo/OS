/*************************************************************************
*                            -=-=-= os.c =-=-=-
*
* THIS FILE WAS CREATED FOR ELEC4710 - REALTIME SYSTEMS
* MAJOR PROJECT ASSIGNMENT
*
* This file contains functions that the operating system will issue system
* calls for. 
*
* Compiler: gcc
*
* Author: Colin Goudie
**************************************************************************/
#include <os/config.h>
#include <os/sys.h>
#include <os/taskm/sema.h>

int sema_create(sema_handle* sem_handle, int value)
{
    int result;
    SYSTEM_CALL_FUNC( SYS_SEMA_CREATE, value, &result );
    return result;
}

int sema_wait(sema_handle sem_handle)
{
    int result;
    SYSTEM_CALL_FUNC( SYS_SEMA_WAIT, sem_handle, &result);
    return result;
}

int sema_signal(sema_handle sem_handle)
{
    int result;
    SYSTEM_CALL_FUNC( SYS_SEMA_SIGNAL, sem_handle, &result);
    return result;
}

