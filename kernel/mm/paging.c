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
**************************************************************************/
#include <os/mm/paging.h>

static ulong*   page_directory = (ulong*)0x9C00;
static ulong*   page_table = (ulong*)0x9D00;

init_paging()
{

}