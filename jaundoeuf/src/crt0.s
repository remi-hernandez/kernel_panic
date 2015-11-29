bits 32

global start

extern kstart ; c entry point

section .multiboot
  align 4
  dd 0x1badb002
  dd 0x3
  dd - (0x1badb002 + 0x3)

section .text

start:
  jmp multiboot_entry

multiboot_entry:
  cli
  mov   esp, stack
  push  ebx
  push  eax
  call kstart
  hlt

section .bss
  resb 8192 ; reserve 8 kilobytes of memory here

stack:
