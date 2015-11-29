global idt_flush

idt_flush:
  mov eax, [esp + 4]
  lidt [eax]
  ret

; Macro for interrupt handler which does not push an error (we push a dummy val)
%macro isr_noerr 1
global isr%1

isr%1:
  cli
  push byte 0
  push byte %1
  jmp isr_stub
%endmacro

; Macro for interrupt handler which pushes an error
%macro isr_err 1
global isr%1

isr%1:
  cli
  push byte %1
  jmp isr_stub
%endmacro

isr_noerr 0
isr_noerr 1
isr_noerr 2
isr_noerr 3
isr_noerr 4
isr_noerr 5
isr_noerr 6
isr_noerr 7
isr_err   8
isr_noerr 9
isr_err   10
isr_err   11
isr_err   12
isr_err   13
isr_err   14
isr_noerr 15
isr_noerr 16
isr_noerr 17
isr_noerr 18
isr_noerr 19
isr_noerr 20
isr_noerr 21
isr_noerr 22
isr_noerr 23
isr_noerr 24
isr_noerr 25
isr_noerr 26
isr_noerr 27
isr_noerr 28
isr_noerr 29
isr_noerr 30
isr_noerr 31
isr_noerr 80

extern isr_handler

isr_stub:
  pusha ; Push edi, esi, ebp, esp, ebx, edx, ecx, eax

  mov ax, ds ; Lower 16-bits of eax = ds
  push eax ; Save the data segment descriptor
  mov esi, eax

  mov ax, 0x10 ; Load the kernel data segment descriptor
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov gs, ax

  call isr_handler

  pop eax ; Reload the original data segment descriptor
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov gs, ax

  popa ; Pops edi, esi, ebp...
  add esp, 8  ; Cleans up pushed error code and pushed ISR number
  sti
  iret ; Pops 5 things at once: CS, EIP, EFLAGS, SS and ESP!

; Interrupt Request Handlers

%macro irq 2
global irq%1

irq%1:
  cli
  push byte 0
  push byte %2
  jmp irq_stub
%endmacro

irq 0,  32
irq 1,  33
irq 2,  34
irq 3,  35
irq 4,  36
irq 5,  37
irq 6,  38
irq 7,  39
irq 8,  40
irq 9,  41
irq 10, 42
irq 11, 43
irq 12, 44
irq 13, 45
irq 14, 46
irq 15, 47

extern irq_handler

irq_stub:
  pusha           ; Pushes edi, esi, ebp, esp, ebx, edx, ecx, eax

  mov ax, ds      ; Lower 16-bits of eax = ds
  push eax        ; Save the data segment descriptor

  mov ax, 0x10    ; Load the kernel data segment descriptor
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov gs, ax

  call irq_handler

  pop eax         ; Reload the original data segment descriptor
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov gs, ax

  popa            ; Pops edi, esi, ebp...
  add esp, 8      ; Cleans up pushed error code and pushed ISR number.

  sti
  iret            ; Pops 5 things at once: CS, EIP, EFLAGS, SS and ESP!
