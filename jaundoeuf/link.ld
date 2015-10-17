ENTRY(start)
OUTPUT_FORMAT(elf32-i386)
OUTPUT_ARCH(i386)
SECTIONS
{
  /* The kernel will live at 3Gb + 1Mb in the virtual address space,
     which will be mapped to 1Mb in the physical address space.
  */

  . = 0xC0100000;

  .text /* ALIGN(0x1000) : AT(ADDR(.text) - 0xC0000000) */ :
  {
    *(.multiboot_header)
      *(.text)
  }

  .rodata /* ALIGN(0x1000) : AT(ADDR(.rodata) - 0xC0000000) */ :
  {
    *(.rodata)
  }

  .data /* ALIGN(0x1000) : AT(ADDR(.data) - 0xC0000000) */ :
  {
    *(.data)
  }

  .bss /* ALIGN(0x1000) : AT(ADDR(.bss) - 0xC00000000) */ :
  {
    *(.bss)
  }
}
