/*************************************************************************
 *                        -=-=-= console.h =-=-=-
 *
 * THIS FILE WAS CREATED FOR ELEC4710 - REALTIME SYSTEMS 
 * MAJOR PROJECT ASSIGNMENT
 * 
 * This file contains definitions and constants that are used in the 
 * GUI/Console of the Operating System
 *
 * Compiler: gcc
 *
 * Author: Colin Goudie
 *************************************************************************/
#ifndef __CONSOLE_H__
#define __CONSOLE_H__

#define             VIDEO_MEMORY             0xb8000

/* Foreground and Background Console Colours */
#define             BLACK                    0x0
#define             BLUE                     0x1
#define             GREEN                    0x2
#define             CYAN                     0x3
#define             RED                      0x4
#define             MAGENTA                  0x5
#define             BROWN                    0x6
#define             WHITE				     0x7
/* Foreground Console Colours Only */
#define             DARK_GREY                0x8
#define             BRIGHT_BLUE              0x9
#define             BRIGHT_GREEN             0xA
#define             BRIGHT_CYAN              0xB
#define             PINK                     0xC
#define             BRIGHT_MAGENTA           0xD
#define             YELLOW                   0xE
#define             BRIGHT_WHITE             0xF

 

#define             CONSOLE_WIDTH            80
#define             CONSOLE_HEIGHT           25
#define             CONSOLE_DEPTH            2

#endif
