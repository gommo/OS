/*************************************************************************
*                            -=-=-= os.h =-=-=-
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
#ifndef __OS_H__
#define __OS_H__

#include <os/taskm/sema.h>

int sema_create(sema_handle* sem_handle, int value);

int sema_wait(sema_handle sem_handle);

int sema_signal(sema_handle sem_handle);

#endif

