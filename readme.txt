					-=-=-= ELEC4710 - Realtime Systems =-=-=-

					  	    	-=-=-= README.TXT =-=-=-

									By Colin Goudie ( 9805806 )

The kernel.bin file that is created must be added to the /boot/ directory of a grub image file or floppy disk that has grub installed on it.

The easiest thing to do is get the grub_disk.img file from 
http://www.pro-pos.org/downloads/grub_disk.zip

Place the grub_disk.img file into a directory (/home/user/os)

Set it up for mounting (run command below from /home/user/os

$ losetup /dev/loop0	grub_disk.img

Make a temp directory in the current directory
$ mkdir temp  (/home/user/os/temp)

Mount

$ mount /dev/loop0 temp/

Then you can simply compile this OS with 

$ make

This creates a file called kernel.bin

$ cp kernel.bin temp/boot

Unmount

$ umount temp/

You know have grub_disk.img that contains the bootable OS.

Run BOCH'S i386 emulator or 'dd' the img to a floppy disk


