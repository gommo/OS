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

/**
 * Initialises and turns on paging
 */
void init_paging();

#endif