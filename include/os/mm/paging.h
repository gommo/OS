/*************************************************************************
*                             -=-=-= paging.h =-=-=-
*
* THIS FILE WAS CREATED FOR ELEC4710 - REALTIME SYSTEMS
* MAJOR PROJECT ASSIGNMENT
*
* This file contains functions and structures to enable and manage the
* paging mechanism of the operating system
*
* Compiler: gcc
*
* Author: Colin Goudie
*
* Notes: See Intel Arch Dev Manual 3 S:3.6-3.7
**************************************************************************/
#ifndef __PAGING_H__
#define __PAGING_H__

#include <os/config.h>
#include <os/multiboot.h>

/** Page size for this processor */
#define     PAGE_SIZE           4096

/**
 * Page Directory entry
 * (Intel Dev Manual #3, S:3.6 Fig 3-14)
 */
struct page_directory_entry
{
    unsigned present:1;                 /* 1 - Currently loaded in memory, 0 - needs to be fetched */
    unsigned read_write:1;              /* 1 - Read/Write, 0 - Read Only */
    unsigned user_supervisor:1;         
    unsigned write_through:1;
    unsigned cache_disabled:1;
    unsigned accessed:1;
    unsigned zero:1;                            /* Reserved, set to 0 */
    unsigned page_size_granularity:1;           /* 0 - 4Kbs, 4 - 4Mb */
    unsigned global_page:1;                     /* Ignored */
    unsigned available:3;                       
    unsigned page_table_base_addr:20;           /* 20bit page table base address */
};

/** 
 * Page Table Entry
 * (Intel Dev Manual #3, S:3.6 Fig 3-14)
 */
struct page_table_entry
{
    unsigned present:1;
    unsigned read_write:1;
    unsigned user_supervisor:1;
    unsigned write_through:1;
    unsigned cache_disabled:1;
    unsigned accessed:1;
    unsigned dirty:1;
    unsigned page_table_attribute_index:1;
    unsigned global_page:1;
    unsigned available:3;
    unsigned page_base_addr:20;                 /* 20bit page base address */
};

/**
 * Structure describing the layout of a page directory
 */
struct page_directory
{
    struct page_directory_entry entry[1024];
};

/**
 * Structure describing the layout of a page table
 */
struct page_table
{
    struct page_table_entry entry[1024];
};

/**
 * Initialises and turns on paging
 */
void init_paging();

void k_initial_page_fill(multiboot_info_t* info, ulong kernel_page_directory, ulong kernel_page_table);

#endif
