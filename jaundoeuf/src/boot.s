bits 32

section .text
  align 4
  dd 0x1badb002
  dd 0x0
  dd - (0x1badb002 + 0x0)

global start
global rd_port
global wr_port

extern kstart ; c entry point

rd_port:
  mov edx, [esp + 4]
  in  al, dx
  ret

wr_port:
  mov edx, [esp + 4]
  mov al,  [esp + 4 + 4]
  out dx,  al
  ret

start:
  cli
  mov  esp, stack_space
  push ebx
  push eax
  call kstart
  hlt

section .bss
  resb 8192

stack_space:
