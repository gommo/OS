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
/** Holds the current sema_handle number to hand out */
static sema_handle next_handle = 1;

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
        sema->handle = next_handle++;
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
        sema->handle = next_handle++;
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
    thread_t* current;
    interrupts_enabled = return_interrupt_status();
    disable();

    klprintf(5, "%s in semaphore_wait", get_current_task()->name);
    klprintf(6, "                                                               ");

    current = get_current_thread();
    //First find this semaphore
    int i=1;
    semaphore_t* sema = sema_head;

    while (i < (int)sem_handle)
    {
        if (sema == NULL)
            break;

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
    sema->value--;

    //As we have at least tried to enter in this critical section, ensure this thread
    //knows its the one in this critical section
    current->sema = sema;

    if (sema->value < 0)
    {
        //Need to block the current thread on this semaphore
        thread_t* block_list = sema->blocked_list;
        
        if (block_list == NULL) 
        {
            //No threads currently blocked on this semaphore
            sema->blocked_list = current;
            current->sprev = sema->blocked_list;
            current->snext = NULL;
        }
        else
        {
            while (block_list->snext != NULL) {
                block_list = block_list->snext;
            }

            //Ok block_list now points to last thread blocked on this semapore, need to add
            //this current thread to this list
            current->sprev = block_list;
            block_list->snext = current;
            current->snext = NULL;
        }

        //remove current from its queue
        remove_current_thread_from_running_queues();

        //Before we shedule we need to set currrent thread to blocked for the scheduler
        current->parent_process->state = TASK_STOPPED;
        
        //Then call schedule
        schedule();

        //Hopefully when we are here means that we have become unblocked and 
        //now can have the semaphore, hence we drop through to the code below
    }

    //We are successfully in this critical section, we can successfully finish this
    //function call

    if (interrupts_enabled)
        enable();


    return SUCCESS;
}

int semaphore_signal(sema_handle sem_handle)
{
    char interrupts_enabled;
    thread_t* current;
    interrupts_enabled = return_interrupt_status();
    disable();

    klprintf(5, "                                                               ");
    klprintf(6, "%s in semaphore_signal", get_current_task()->name);

    current = get_current_thread();
    //First find this semaphore
    int i=1;
    semaphore_t* sema = sema_head;

    while (i < (int)sem_handle)
    {
        if (sema == NULL)
            break;

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
    //critical section

    if (current->sema == sema)
    {
        //Increment the semaphores value
        sema->value ++;

        //Ensure we don't go over 1
        if (sema->value > 1)
            sema->value = 1;

        //We need to unblock threads on this semaphore
        if (sema->blocked_list == NULL) 
        {
            //No threads blocked on this semaphore, do nothing
        }
        else
        {
            //Lets unblock sema->blocked_list head
            //Unblock one of the threads
            thread_t* head_of_blocked = sema->blocked_list;
            sema->blocked_list = head_of_blocked->snext;

            //Add head_of_blocked to the tail of the correct priority queue
            add_thread_to_running_queues(head_of_blocked);

            head_of_blocked->parent_process->state = TASK_READY;
        }
    }
    else
    {
        return FAILURE;
    }

    //We are leaving the critical section
    current->sema = NULL;

    if (interrupts_enabled)
        enable();

    return SUCCESS;
}

int semaphore_destroy(sema_handle sem_handle)
{

    return SUCCESS;
}

