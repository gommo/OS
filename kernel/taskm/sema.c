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
#include <os/mm/mm.h>
#include <os/kernel.h>

/* This is the head of our semaphore list */
static semaphore_t* sema_head = NULL;
/* This is the tail of our semaphore list */
static semaphore_t* sema_tail = NULL;

int semaphore_create(sema_handle* sem_handle, int value)
{
    char interrupts_enabled;

    //Need to create a new semaphore
    semaphore_t* sema = (semaphore_t*)k_malloc(sizeof(semaphore_t));

    if (sema == NULL)
    {
        //Could not create a semaphore
        return FAILURE;
    }

    interrupts_enabled = return_interrupt_status();
    disable();

    if (sema_tail == NULL) //First semaphore entry
    {
        //Initialise this new semaphore
        sema->handle = 1;
        sema->value = value;
        sema->blocked_list = NULL;
        
        sema->next = NULL;
        sema->prev = NULL;
        //Add to our list
        sema_head = sema_tail = sema;
    }
    else
    {
        //Initialise this new semaphore
        sema->handle = sema_tail->handle + 1;
        sema->value = value;
        sema->blocked_list = NULL;

        //Add to our list
        sema->next = NULL;
        sema->prev = sema_tail;
        sema_tail->next = sema;
        sema_tail = sema;
    }

    //Return the Handle
    *sem_handle = sema->handle;

    if (interrupts_enabled)
        enable();

    return SUCCESS;
}

int semaphore_wait(sema_handle sem_handle)
{
    char interrupts_enabled;
    interrupts_enabled = return_interrupt_status();
    disable();

    //First find this semaphore
    int i=1;
    semaphore_t* sema = NULL;

    while (i < (int)sem_handle)
    {
        if (sema->next != NULL)
        {
            sema = sema->next;
        }

        i++;
    }

    if (i != (int)sem_handle)
    {
        //We didn't find this semaphore
        return FAILURE;
    }

    //Now decrement the sema_handles value
    sema->value --;

    if (sema->value < 0)
    {
        //Need to block the current thread on this semaphore

    }

    //We are successfully in this critical section, ensure this thread
    //knows its the one in this critical section

    get_current_thread()->sema = sema;

    if (interrupts_enabled)
        enable();


    return SUCCESS;
}

int semaphore_signal(sema_handle sem_handle)
{
    char interrupts_enabled;
    interrupts_enabled = return_interrupt_status();
    disable();

    //First find this semaphore
    int i=1;
    semaphore_t* sema = NULL;

    while (i < (int)sem_handle)
    {
        if (sema->next != NULL)
        {
            sema = sema->next;
        }

        i++;
    }

    if (i != (int)sem_handle)
    {
        //We didn't find this semaphore
        return FAILURE;
    }


    //Can only signal the semaphore if this thread is actually in this
    //crictial section

    if (get_current_thread()->sema == sema)
    {
        //Increment the semaphores value
        sema->value ++;

        //Ensure we don't go over 1
        if (sema->value > 1)
            sema->value = 1;
    }
    else
    {
        return FAILURE;
    }

    //We are leaving the critical section
    get_current_thread()->sema = NULL;

    if (interrupts_enabled)
        enable();

    return SUCCESS;
}