/*************************************************************************
*                            -=-=-= sema.h =-=-=-
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
#ifndef __SEMA_H__
#define __SEMA_H__

#include <os/config.h>
#include <os/taskm/sched.h>

typedef int sema_handle;
/**
 * This structure describes a semaphore
 */
typedef struct semaphore
{
    /** 
     * Value of the semaphore (1 = available; 0 = Task got it, < 0 # of 
     * blocked tasks waiting on it 
     */
    uint value;
    /** Handle for this semaphore */
    int handle;
    /* Pointers to go through list of semaphores in OS */
    struct semaphore* next;
    struct semaphore* prev;
    /**
     * Ptr to the list of blocked threads on this semaphore
     */
    struct thread* blocked_list;

} semaphore_t;

/** Some Semaphore Functions */

/**
 * Creates a new semaphore
 *
 * @param semaphore_t* Pointer to the semaphore returned by reference
 * @return SUCCESS or FAILURE
 */
int semaphore_create(sema_handle* sem_handle, int value);
/**
 * Waits on a semaphore
 *
 * @param sema_handle semaphore to wait on 
 * @return SUCCESS or FAILURE
 *
 * @todo add timeout
 */
int semaphore_wait(sema_handle sem_handle);
/**
 * Function to signal the semaphore
 *
 * @param sema_handle semaphore to signal
 * @return SUCCESS or FAILURE
 */
int semaphore_signal(sema_handle sem_handle);


#endif

