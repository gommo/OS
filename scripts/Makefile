PROG = kernel.bin
SRCS = kernel.c
OBJS = $(SRCS:.c=.o)
AS = nasm
ASFLAGS = -f elf
CC = cc
CFLAGS = -fwritable-strings
LD = ld
LINKFILE = link.ld
LDFLAGS = -T $(LINKFILE)

all: boot $(OBJS)	build

%.o: %.c
				$(CC) -c $(CFLAGS) $*.c -o $*.o

build: $(OBJS)
				$(LD) $(LDFLAGS) -o $(PROG) boot.o $(OBJS)

boot:	boot.S
				$(AS) $(ASFLAGS) $? -o boot.o

clean:
				rm -f *.o $(PROG)

