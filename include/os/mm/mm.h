/*************************************************************************
*                             -=-=-= mm.h =-=-=-
*
* THIS FILE WAS CREATED FOR ELEC4710 - REALTIME SYSTEMS
* MAJOR PROJECT ASSIGNMENT
*
* This file contains functions and structures used to perform memory 
* management for our operating system
*
* Compiler: gcc
*
* Author: Colin Goudie
**************************************************************************/
#ifndef __MM_H__
#define __MM_H__

#include <string.h>
#include <os/config.h>

//=============
// Dynamic Memory #defines and functions

#define     KERNEL_HEAP_SIZE            (2 * 1024 * 1024)
#define     KERNEL_HEAP_SIZE_IN_UINT    (2 * 1024 * 1024 / 4)
#define     ALLOCATION_SIZE             (512)

#define     CHUNK_ALLOCATED_MASK        0x80000000

void* k_malloc(size_t size);
void k_free(void* ptr);

// End Dynamic memory
//==============

void init_mm();

#endif

