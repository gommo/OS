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

static uint* heap_ptr = (uint*)0x300000;

/**
* Initialises the Dynamic Memory Manager
*/
void init_mm()
{
    //Set the first quad-word of the heap to indicate an unused chunk of KERNEL_HEAP_SIZE
    *heap_ptr = 0;
    *heap_ptr |= KERNEL_HEAP_SIZE;
}
/**
 * Kernel level malloc. This function allocates free memory from the 
 * heap
 *
 * @param size Size in bytes to allocate
 * @return Pointer to the allocated memory
 */
void* k_malloc(size_t size)
{
    //REMEMBER, size is in bytes but our ptr that moves over our heap is in 
    //unsigned int (4 bytes on our i386 machine)
    //The chunk size that is stored in our chunk headers is an uint too
    //We allocate chunks that are double-word aligned
    uint chunk_size;
    uchar remainder;
    uint new_size;

    uint* multi_chunk_first;
    uint multi_chunk_size;
    uint multi_chunk_count;
    //Start looking for a free chunk from beginning of heap
    //We will do a fit_first type algorithm
    uint* ptr = heap_ptr;  
    multi_chunk_first = (uint*)0;
    multi_chunk_size = 0;
    multi_chunk_count = 0;


    remainder = size % sizeof(uint);
    new_size = size / sizeof(uint);
    //If we have a remainder we need an extra 4bytes for it
    if (remainder > 0)
        new_size++;
 
    while (ptr < (heap_ptr + KERNEL_HEAP_SIZE_IN_UINT))
    {
        chunk_size = (*ptr) & ~CHUNK_ALLOCATED_MASK;
     
        if (((*ptr) & CHUNK_ALLOCATED_MASK) == CHUNK_ALLOCATED_MASK)
        {
            //We can't join any multichunks together since we've run into 
            //an allocated chunk
            multi_chunk_first = (uint*)0;
            multi_chunk_size = 0;
            multi_chunk_count = 0;
            //This chunk is allocated jump over it to the next
            //chunk header
            ptr += chunk_size + 1;
        }
        else
        {
            //Is this chunk big enough

            if (chunk_size >= new_size)
            {
                //Excellent we have enough space here
                //Set the allocation bit and size of the new chunk
                (*ptr) = new_size;
                (*ptr) |= CHUNK_ALLOCATED_MASK;
                if (chunk_size != new_size)
                {
                    //We have to add a new chunk header after the allocated space
                    //And set it to unallocated
                    *(ptr + 1 + new_size) = chunk_size - new_size - 1;
                    *(ptr + 1 + new_size) &= ~CHUNK_ALLOCATED_MASK;
                }

                //Also, if we are collecting multiple chunks there is no
                //need since we've found a chunk big enough
                multi_chunk_size = 0;
                multi_chunk_count = 0;
                multi_chunk_first = (uint*)0;

                //Return the pointer to the allocated space
                return (void*)(ptr+1);            
            }
            else
            {
                //No, we don't have enough space in this chunk
                if (multi_chunk_count == 0)
                {
                    multi_chunk_first = ptr;
                    multi_chunk_size = chunk_size;
                    multi_chunk_count = 1;
                    ptr += chunk_size + 1;
                    continue;
                }
                
                //This is the 2nd+ chunk in our multichunk investigation
                multi_chunk_size += chunk_size;

                if (multi_chunk_size >= new_size)
                {
                    //We found a multichunk big enough
                    //Set the allocation size
                    *(multi_chunk_first) = multi_chunk_size + multi_chunk_count;
                    *(multi_chunk_first) |= CHUNK_ALLOCATED_MASK;
                    return (void*)(multi_chunk_first+1);
                }

                multi_chunk_size += chunk_size;
                multi_chunk_count++;
                ptr += chunk_size + 1;
            }
        }
    }

    return NULL;
}

/**
 * Frees memory allocated by k_malloc and returns it to the heap
 *
 * @param ptr Ptr to the section of memory to free
 */
void k_free(void* ptr)
{
    //Simply unallocated this chunk
    if ((uint)ptr < (uint)heap_ptr || (uint)ptr > ((uint)heap_ptr + KERNEL_HEAP_SIZE))
    {
        //Pointer is outside our boundary don't do anything
        return;
    }

    if ((*(((uint*)ptr)-1) & CHUNK_ALLOCATED_MASK ) != CHUNK_ALLOCATED_MASK)
    {
        //The chunk header is not allocated so this pointer has already been
        //unallocated or we may be pointing to wrong memory, don't
        //do anything
        return;
    }

    //Unallocate
    *(((uint*)ptr)-1) &= ~CHUNK_ALLOCATED_MASK;
}
