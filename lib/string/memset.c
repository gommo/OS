/*************************************************************************
*                          -=-=-= memset.c =-=-=-
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
*       The actual functions needed by the C Library were referenced from
*       http://www.acm.uiuc.edu/webmonkeys/book/c_guide/
**************************************************************************/
#include <string.h>

void*   memset( void* str, int c, size_t n )
{
    unsigned int i;
    //unsigned char* ptr = str;

    for (i=0; i < n; i++)
    {
        ((unsigned char*)str)[i] = (unsigned char)c;
    }

    return str;
}
