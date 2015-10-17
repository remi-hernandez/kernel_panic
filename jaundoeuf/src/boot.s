[BITS 32]
[EXTERN kstart] ; Kernel C start function

HEADER_MAGIC	equ 0x1badb002
HEADER_FLAGS	equ 0x00
HEADER_CHECK	equ -(HEADER_MAGIC + HEADER_FLAGS)
VIRTUAL_BASE	equ 0xC0000000

[SECTION .text]
ALIGN		4
multiboot_header:
  dd		HEADER_MAGIC
  dd		HEADER_FLAGS
  dd		HEADER_CHECK

[GLOBAL start] ; Kernel ASM entry point
start equ _start
_start:
  add  ebx, VIRTUAL_BASE
  push esp     ; stack address
  push ebx     ; multiboot information
  push eax     ; magic
  call kstart  ; kernel entry point
  jmp  $       ; infinite loop
