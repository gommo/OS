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

#define         NO_TTY                          -1
#define         TTY_0                           0
#define         TTY_1                           1
#define         TTY_2                           2
#define         TTY_3                           3

#define         TTY_WIDTH                       80
#define         TTY_HEIGHT                      25
#define         BYTES_PER_CHARACTER             2

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
    /** TTY Number */
    uint number;
    /** This is the screen page in memory, currently we'll only have one */
    uchar screen_page[TTY_WIDTH * TTY_HEIGHT * BYTES_PER_CHARACTER];
    /** Line number we are at */
    uint line_number;
};

/**
 * Initialises the tty module 
 */
void init_tty();

/**
 * Returns a ptr to the tty device specified by the input number
 *
 * @param uint TTY number
 * @return ptr to the tty device
 */

}

#endif
