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
    /**
     * Ptr to the list of blocked threads on this semaphore
     */
    struct thread* blocked_list;

} semaphore_t;

/** Some Semaphore Functions */

/**
 * Initialises the semaphore
 *
 * @param semaphore_t* Pointer to the semaphore returned by reference
 * @return SUCCESS or FAILURE
 */
int semaphore_init(semaphore_t* sema_handle, int value);
/**
 * Waits on a semaphore
 *
 * @param semaphore_t* semaphore to wait on 
 * @return SUCCESS or FAILURE
 *
 * @todo add timeout
 */
int semaphore_wait(semaphore_t* sema_handle);
/**
 * Function to signal the semaphore
 *
 * @param semaphore_t* semaphore to signal
 * @return SUCCESS or FAILURE
 */
int semaphore_signal(semaphore_t* sema_handle);

#endif