/*************************************************************************
 *                           -=-=-= config.h =-=-=-
 *
 * THIS FILE WAS CREATED FOR ELEC4710 - REALTIME SYSTEMS
 * MAJOR PROJECT ASSIGNMENT
 *
 * This file contains defines that are used to configure the operating system
 * and also conventient typedef's
 *
 * Compiler: gcc
 *
 * Author: Colin Goudie
 **************************************************************************/
#ifndef __CONFIG_H__
#define __CONFIG_H__

/* Some typedefs to ease finger ache */
typedef unsigned int uint;
typedef unsigned char uchar;
typedef unsigned long ulong;
typedef unsigned short ushort;

typedef enum {FALSE=0, TRUE=1} BOOL;

#define TRUE    ((uchar)1)
#define FALSE   ((uchar)0)
#define NULL    ((void*)0)

#define PACK __attribute__   ((pack))


#endif

