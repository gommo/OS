/*************************************************************************
*                          -=-=-= stdarh.h =-=-=-
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
#ifndef __STDARG_H__
#define __STDARG_H__

typedef char * va_list;

#define va_rounded_size(type) \
    (((sizeof (type) + sizeof (int) - 1) / sizeof (int)) * sizeof (int))

#define va_start(ap, v) \
    ((void) (ap = (va_list) &v + va_rounded_size (v)))

#define va_arg(ap, type) \
    (ap += va_rounded_size (type), *((type *)(ap - va_rounded_size (type)))) 

#define va_end(ap) ((void) (ap = 0))

#endif
