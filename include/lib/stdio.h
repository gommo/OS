/*************************************************************************
*                          -=-=-= stdio.h =-=-=-
*
* THIS FILE WAS CREATED FOR ELEC4710 - REALTIME SYSTEMS
* MAJOR PROJECT ASSIGNMENT
*
* The stdio header provides functions for performing input and output.
*
* This file contains standard ISO C library functions. They were not 
* written by me nor do I take credit for them. They are included so 
* hopefully this OS will be able to compile ANSI C programs.
*
* Compiler: gcc
* 
* Note: MUCH OF THIS CODE IS BASED ON GPL CODE AVAILABLE FROM THE LIBC 
*       LIBRARY AND THE LINUX KERNEL
*       The actual functions needed by the C Library were referenced from
*       http://www.acm.uiuc.edu/webmonkeys/book/c_guide/
**************************************************************************/
#ifndef __STDIO_H__
#define __STDIO_H__

#ifndef     NULL
#define     NULL        ((void*)0)
#endif

#define     EOF         (-1)

int vsprintf( char *buf, const char *fmt, va_list args );

#endif
