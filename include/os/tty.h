/*************************************************************************
*                             -=-=-= tty.h =-=-=-
*
* THIS FILE WAS CREATED FOR ELEC4710 - REALTIME SYSTEMS
* MAJOR PROJECT ASSIGNMENT
*
* This file contains the functions that coordinate the i/o to the user
* via the screen & keyboard
*
* Compiler: gcc
*
* Author: Colin Goudie
**************************************************************************/
#ifndef __TTY_H__
#define __TTY_H__

/**
 * This structure contains the information needed about a tty device
 *
 * A tty device is used to display information to the user and get 
 * information from the user. I'm hoping that my OS will have say
 * 4 tty devices on boot up and you can switch to any and issue
 * commands
 */
struct tty
{
    uint number;                // tty number

};

#endif
