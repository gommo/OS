/* OUTPUT_FORMAT("binary") */
ENTRY(start)
SECTIONS
{
   . = 0x00100000;

   .text :
   {
      *(.text)
      *(.rodata)
   }

   .data ALIGN (0x1000) :
   {
      *(.data);
   }

   .bss :
   {
      _sbss = .;
      *(COMMON)
      *(.bss)
      _ebss = .;
   }
}
