/*************************************************************************
*                          -=-=-= string.h =-=-=-
*
* THIS FILE WAS CREATED FOR ELEC4710 - REALTIME SYSTEMS
* MAJOR PROJECT ASSIGNMENT
*
* This file contains standard ISO C library functions. They were not 
* written by me nor do I take credit for them. They are included so 
* hopefully this OS will be able to compile ANSI C programs.
*
* Compiler: gcc
* 
* Note: MUCH OF THIS CODE IS BASED ON GPL CODE AVAILABLE FROM THE LIBC 
*       LIBRARY AND THE LINUX KERNEL
**************************************************************************/
#ifndef __STRING_H__
#define __STRING_H__

#include <os/config.h>

ulong strlen( const char *str );
uint strnlen( const char *s, uint count );

#endif
