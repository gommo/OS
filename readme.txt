					-=-=-= ELEC4710 - Realtime Systems =-=-=-
                
                        -=-=-= README.TXT =-=-=-

                        By Colin Goudie ( 9805806 )
					
					
This operating system should be compiled on a GNU Operating System using
GCC, and the Gnu Assembler, using gmake.

Notes on the example
--------------------
user.c contains the code that can be used to varify certain features of this 
kernel.

We have 2 Normal priority threads (test_function and test_function2) started
from kernel.c. These ensure both semaphores and our timer is working by attempting
to enter the critical section defined between sema_wait(sema) and sema_signal(sema).
Both these threads use the same semaphore so effectly they block each other each time.
When entering the critical sections they sleep for a few hundred milliseconds.

Before test_function2 actually starts it also starts a higher priority thread. This thread
continually loops printing out its current loop count. When it passed 15,000 iterations the
thread dies.

The important thing here to note is that the scheduler avoids starvation of the lower
priority threads by not allowing the high priority task to hog the CPU all the time.

test_function also starts a realtime thread that executions for 500 iterations before dying.
This demonstrates the dynamic memory (i.e. thread creation and destruction) and also that
the realtime thread takes preference over all threads with no limit to the amount of 
time slicing it can receive. 

This kernel also supports dynamic memory via the k_malloc and k_free commands. Threads 
can also die by 

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


