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
SRCS = kernel/kernel.c
OBJS = $(SRCS:.c=.o)
INCS = -I./include -I./
AS = gcc
ASFLAGS = -nostdinc
CC = cc
CFLAGS = -fwritable-strings -nostdinc -Wall -O2
LD = ld
LINKFILE = link.ld
LDFLAGS = -nostdlib -N -Ttext 100000

all: boot init $(OBJS)	build

%.o: %.c
				$(CC) -c $(CFLAGS) $(INCS) $*.c -o $*.o

build: $(OBJS)
				$(LD)  $(LDFLAGS) -o $(PROG) boot.o init.o $(OBJS)

boot:	boot.S
				$(AS) -c $(INCS) $(ASFLAGS) $? -o boot.o

init:	init.S
				$(AS) -c $(INCS) $(ASFLAGS) $? -o init.o
clean:
				rm -f *.o $(OBJS) $(PROG)

