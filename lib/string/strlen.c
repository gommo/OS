/*************************************************************************
*                          -=-=-= strlen.c =-=-=-
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
#include <string.h>

unsigned int strlen( const char *str )
{
/*    const char *ret = str;

    while ( *str++ );

    return str - 1 - ret;*/
    const char *sc;

    for ( sc = str; *sc != '\0'; ++sc );

    return sc - str;
}
