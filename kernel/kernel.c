/*************************************************************************
 *                            -=-=-= kernel.c =-=-=-
 *
 * THIS FILE WAS CREATED FOR ELEC4710 - REALTIME SYSTEMS
 * MAJOR PROJECT ASSIGNMENT
 *
 * This file contains the kernel main function that is used to initialise
 * the realtime kernel/Operating System 
 *
 * Compiler: gcc
 *
 * Author: Colin Goudie
 **************************************************************************/
#include <os/kernel.h>
#include <os/platform.h>
#include <asm/io.h>
#include <console.h>

#define HZ          100
#define LATCH       (1193180/HZ)

extern int timer_interrupt(void);
void test();
void k_clear_screen();
unsigned int k_printf(char *message, unsigned int line);

int k_main() // like main in a normal C program
{
	k_clear_screen();

    k_printf("k_main() started", 0);

    set_interrupt_gate(0x20, &test);
    
    outb(0x36,0x43);    // Binary, Mode3,
    outb(LATCH & 0xff, 0x40);   //LSB
    outb(LATCH >> 8, 0x40);

    while(1){}
    
    return 0;    
};

void test()
{
    k_printf("Got Timer", 10);
}

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

