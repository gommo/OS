/*************************************************************************
 *                            -=-=-= kernel.c =-=-=-
 *
 * THIS FILE WAS CREATED AND IS FOR ELEC4710 - REALTIME SYSTEMS
 * PROJECT ASSIGNMENT
 *
 * This file contains the kernel main function that is used to initialise
 * the realtime kernel/Operating System 
 *
 * Compiler: gcc
 *
 * Author: Colin Goudie
 **************************************************************************/
#include <console.h>

void k_clear_screen();
unsigned int k_printf(char *message, unsigned int line);


k_main() // like main in a normal C program
{
	k_clear_screen();
	k_printf("Hi!\nHow's this for a starter OS?", 0);
};

void k_clear_screen() // clear the entire text screen
{
	char *vidmem = (char *) VIDEO_MEMORY;
	unsigned int i=0;
	while(i < (CONSOLE_WIDTH * CONSOLE_HEIGHT * CONSOLE_DEPTH))
	{
		vidmem[i]=' ';
		i++;
		vidmem[i]=WHITE;
		i++;
	};
};

unsigned int k_printf(char *message, unsigned int line) // the message and then the line #
{
	char *vidmem = (char *) VIDEO_MEMORY;
	unsigned int i=0;

	i=(line * CONSOLE_WIDTH * CONSOLE_DEPTH);

	while(*message!=0)
	{
		if(*message=='\n') // check for a new line
		{
			line++;
			i=(line * CONSOLE_WIDTH * CONSOLE_DEPTH);
			*message++;
		} else {
			vidmem[i]=*message;
			*message++;
			i++;
			vidmem[i]=WHITE;
			i++;
		};
	};

	return(1);
};

/**
 * Formats a string and prints it on the screen ,just like libc printf
 * 
 * @param const char* String format to print
 * @param ... Variable parameter list
 */
void printf(const char* format, ...)
{

}
