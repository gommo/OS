/*************************************************************************
*                        -=-=-= strcpy.c =-=-=-
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
char *strcpy( char *str1, const char *str2 )
{
    char *retval = str1;

    while ( *str2 != '\0' )
    {
        *str1++ = *str2++;
    }
    *str1++ = *str2++;

    return retval;
}
