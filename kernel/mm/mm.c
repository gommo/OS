/*************************************************************************
*                             -=-=-= mm.c =-=-=-
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
/*
NOTES

We are not using paging initially so our kernel resides @ 0x100000 (1MB)

We want to create a k_malloc, k_free functions so lets put the kernel heap
at say 0x300000 (3MB) This should give us plenty of room. We'll make the heap
say 2MB (0x300000 - 0x500000)

*/
#include <os/mm/mm.h>

static void* heap_ptr = 0x300000;
static uchar k_allocation_table[KERNEL_HEAP_SIZE / ALLOCATION_SIZE / 0x8 ];

void init_mm()
{
    //Clear out the allocation table
    memset( k_allocation_table, 0, sizeof(k_allocation_table));

}


void* k_malloc(size_t size)
{
    uint index;

    uint starting_alloc_index = 0;
    uint starting_alloc_bit = 0;
    // This k_malloc will simply allocate memory by looping through the k_allocation_table
    // looking for a consecutive space thats big enough to fit the object into.
    
    // Note that each bit of the k_allocation_table represents 512Bytes of space
    for (index = 0; index < sizeof(k_allocation_table), i++)
    {

    }
    
    return heap_ptr;
}

void k_free(void* ptr)
{

}
