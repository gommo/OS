/*************************************************************************
*                          -=-=-= string.h =-=-=-
*
* THIS FILE WAS CREATED FOR ELEC4710 - REALTIME SYSTEMS
* MAJOR PROJECT ASSIGNMENT
*
* The string header provides many functions useful for manipulating 
* strings (character arrays).
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
#ifndef __STRING_H__
#define __STRING_H__

#ifndef     NULL
#define     NULL        ((void*)0)
#endif

typedef unsigned int    size_t;
/*
void*   memchr( const void* str, int c, size_t n );
int     memcmp( const void& str1, const void* str2, size_t n );
*/
void*   memcpy( void* str1, const void* str2, size_t n );
/*
void*   memmove( void* str1, const void* str2, size_t n );
*/
void*   memset( void* str, int c, size_t n );
/*
char*   strcat( char* str1, const char* str2 );
char*   strncat( char* str1, const char* str2, size_t n );
char*   strchr( const char* str, int c );
int     strcmp( const char* str1, const char* str2 );
int     strncmp( const char* str1, const char* str2, size_t n );
int     strcoll( const char* str1, const char* str2 );
*/
char*   strcpy( char* str1, const char* str2 );
/*
char*   strncpy( char* str1, const char* str2, size_t n );
size_t  strcspn( const char* str1, const char* str2 );
char*   strerror( int errnum );
*/
size_t strlen( const char* str );
/*
char*   strpbrk( const char* str1, const char* str2 );
char*   strrchr( const char* str, int c );
size_t  strspn( const char* str1, const char* str2 );
char*   strstr( const char* str1, const char* str2 );
char*   strtok( char* str1, const char* str2 );
size_t  strxfrm( char* str1, const char* str2, size_t n );
*/
size_t strnlen( const char* s, unsigned int count );

#endif
