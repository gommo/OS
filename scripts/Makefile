##########################################################################
#                        ELEC4710 - REALTIME SYSTEMS
#
# THIS FILE WAS CREATED AND IS FOR ELEC4710 - REALTIME SYSTEMS
# PROJECT ASSIGNMENT
#
# This file is the Makefile used to build the Operating System
#
# Author: Colin Goudie
#
##########################################################################
PROG = kernel.bin
SRCS = kernel.c
OBJS = $(SRCS:.c=.o)
INCS = -I./include
AS = nasm
ASFLAGS = -f elf
CC = cc
CFLAGS = -fwritable-strings
LD = ld
LINKFILE = link.ld
LDFLAGS = -T $(LINKFILE)

all: boot $(OBJS)	build

%.o: %.c
				$(CC) -c $(CFLAGS) $(INCS) $*.c -o $*.o

build: $(OBJS)
				$(LD) $(LDFLAGS) -o $(PROG) boot.o $(OBJS)

boot:	boot.S
				$(AS) $(ASFLAGS) $? -o boot.o

clean:
				rm -f *.o $(PROG)

