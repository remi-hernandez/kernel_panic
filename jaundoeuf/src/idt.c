#include "idt.h"
#include "stdio.h"
#include "string.h"
#include "registers.h"
#include "io/serial.h"

extern void idt_flush(idt_ptr_t *);

idt_entry_t idt_entries[256];
idt_ptr_t   idt_ptr;

void
init_idt()
{
  idt_ptr.limit = sizeof(idt_entry_t) * 256 -1;
  idt_ptr.base  = (unsigned long) &idt_entries;

  memset(&idt_entries, 0, sizeof(idt_entry_t) * 256);

  printk("Flushing IDT: ");
  idt_flush(&idt_ptr);
  printk(SUCC_COLOR "ok\n" DEF_COLOR);
}

void
idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags)
{
  idt_entries[num].base_lo = base & 0xFFFF;
  idt_entries[num].base_hi = (base >> 16) & 0xFFFF;

  idt_entries[num].sel     = sel;
  idt_entries[num].always0 = 0;
  idt_entries[num].flags   = flags;
}
