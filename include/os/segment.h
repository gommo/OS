/*************************************************************************
*                           -=-=-= segment.h =-=-=-
*
* THIS FILE WAS CREATED FOR ELEC4710 - REALTIME SYSTEMS
* MAJOR PROJECT ASSIGNMENT
*
* This file contains defines for segment related issues
*
* Compiler: gcc
*
* Author: Colin Goudie
*************************************************************************/
#ifndef __SEGMENT_H__
#define __SEGMENT_H__

/* Reference: Intel Arch Dev Manual 3  S:3.4.1-3 */

#define     SINDEX_KERNEL_CODE      1
#define     SINDEX_KERNEL_DATA      2
#define     SINDEX_TSS              3
#define     SINDEX_USER_CODE        4
#define     SINDEX_USER_DATA        5

/* Macro to move our index to the required location in the 
   segment selector */
#define     SINDEX(x)       (x << 3)

#define     TABLE_GDT       (0 << 2)
#define     TABLE_LDT       (1 << 2)

#define     RING0           0
#define     RING1           1  
#define     RING2           2
#define     RING3           3

/* Kernel Code Segment Selector Value */
#define     KERNEL_CODE         ( SINDEX( SINDEX_KERNEL_CODE ) + TABLE_GDT + RING0 )
#define     KERNEL_DATA         ( SINDEX( SINDEX_KERNEL_DATA ) + TABLE_GDT + RING0 )

#define     USER_CODE           ( SINDEX( SINDEX_USER_CODE ) + TABLE_GDT + RING3 )
#define     USER_DATA           ( SINDEX( SINDEX_USER_DATA ) + TABLE_GDT + RING3 )

#endif
