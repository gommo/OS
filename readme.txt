					-=-=-= ELEC4710 - Realtime Systems =-=-=-
                
                        -=-=-= README.TXT =-=-=-

                        By Colin Goudie ( 9805806 )
					
					
This operating system should be compiled on a GNU Operating System using
GCC, and the Gnu Assembler, using gmake.

Notes on this Milestone Build
-----------------------------
This build simply displays three threads. They simply count a variable up 
outputting the current value to the screen. The scheduler simply swaps 
between these two threads.

Currently this kernel isn't really Real-time. I need to have a good look
about what things to add and not get carried away designing a more generic
operating system.

Building the Kernel
-------------------
	
The kernel.bin file that is created must be added to the /boot/ directory of
a grub image file or floppy disk that has grub installed on it. This is 
easily accomplished by following the commands below.

Get the grub_disk.img file from 
http://www.pro-pos.org/downloads/grub_disk.zip

Place the grub_disk.img file into a directory (/home/user/os)

Set it up for mounting (run command below from /home/user/os

$ losetup /dev/loop0	grub_disk.img

Then you can simply compile this OS with 

$ make

This creates a file called kernel.bin
Then run the prep script 

$ prep

You now have grub_disk.img that contains the bootable OS.

Run BOCH'S i386 emulator or 'dd' the img to a floppy disk.

You can also copy the grub_disk.img file to a floppy on 
windows by using a program such as floppy_image.exe


