/*************************************************************************
*                            -=-=-= sema.c =-=-=-
*
* THIS FILE WAS CREATED FOR ELEC4710 - REALTIME SYSTEMS
* MAJOR PROJECT ASSIGNMENT
*
* This file contains code representing semaphores in the operating system
*
* Compiler: gcc
*
* Author: Colin Goudie
**************************************************************************/
#include <os/config.h>
#include <os/taskm/sema.h>

int semaphore_init(semaphore_t* sema_handle, int value)
{
    *sema_handle.value = value;
    

    return SUCCESS;
}

int semaphore_wait(semaphore_t* sema_handle)
{
    //First decrement the sema_handles value
    sema_handle->value --;

    if (sema_handle->value < 0)
    {
        //Need to block the current thread on this semaphore

    }

    //We are successfully in this critical section, ensure this thread
    //knows its the one in this critical section

    get_current_thread()->sema = sema_handle;

    return SUCCESS;
}

int semaphore_signal(semaphore_t* sema_handle)
{
    //Can only signal the semaphore if this thread is actually in this
    //crictial section

    if (get_current_thread()->sema == sema_handle)
    {
        //Increment the semaphores value
        sema_handle->value ++;

        //Ensure we don't go over 1
        if (sema_handle->value > 1)
            sema_handle->value = 1;
    }
    else
    {
        return FAILURE;
    }

    //We are leaving the critical section
    get_current_thread()->sema = NULL;

    return SUCCESS;
}