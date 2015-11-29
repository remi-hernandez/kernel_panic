#include "idt.h"
#include "irq.h"
#include "io/serial.h"
#include "stdio.h"

#define MASTER_COMMAND  0x20
#define MASTER_DATA     0x21
#define SLAVE_COMMAND   0xA0
#define SLAVE_DATA      0xA1

#define ICW1_ICW4       0x01 /* ICW4 (not) needed */
#define ICW1_SINGLE     0x02 /* Single (cascade) mode */
#define ICW1_INTERVAL4  0x04 /* Call address interval 4 (8) */
#define ICW1_LEVEL      0x08 /* Level triggered (edge) mode */
#define ICW1_INIT       0x10 /* Initialization - required! */

#define ICW4_8086       0x01 /* 8086/88 (MCS-80/85) mode */
#define ICW4_AUTO       0x02 /* Auto (normal) EOI */
#define ICW4_BUF_SLAVE  0x08 /* Buffered mode/slave */
#define ICW4_BUF_MASTER 0x0C /* Buffered mode/master */
#define ICW4_SFNM       0x10 /* Special fully nested (not) */

void *
irq_routines[16] =
{
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0
};

void
irq_install_handler(int irq, void (*handler)(registers_t * r))
{
  irq_routines[irq] = handler;
}

void
irq_uninstall_handler(int irq)
{
  irq_routines[irq] = 0;
}

void
irq_remap(uint8_t base)
{
  unsigned char a1, a2;

  a1 = serial_getc(MASTER_COMMAND);                   // Save mask
  a2 = serial_getc(SLAVE_COMMAND);

  serial_putc(MASTER_COMMAND, ICW1_INIT | ICW1_ICW4); // Start initialization sequence
  serial_putc(SLAVE_COMMAND,  ICW1_INIT | ICW1_ICW4); // Start initialization sequence
  serial_putc(MASTER_DATA,    base);                  // Remap to base
  serial_putc(SLAVE_DATA,     base + 8);              // Remap slave to base + 8
  serial_putc(MASTER_DATA,    4);                     // Tell Master about slave @ IRQ 2
  serial_putc(SLAVE_DATA,     2);                     // Tell slave PIC its cascade ID

  serial_putc(MASTER_DATA,    ICW4_8086);
  serial_putc(SLAVE_DATA,     ICW4_8086);

  serial_putc(MASTER_DATA,    a1);                    // Restore mask
  serial_putc(SLAVE_DATA,     a2);
}

void
irq_init(uint8_t base)
{
  irq_remap(base);

  idt_set_gate(32, (unsigned long) irq0, 0x08, 0x8E);
  idt_set_gate(33, (unsigned long) irq1, 0x08, 0x8E);
  idt_set_gate(34, (unsigned long) irq2, 0x08, 0x8E);
  idt_set_gate(35, (unsigned long) irq3, 0x08, 0x8E);
  idt_set_gate(36, (unsigned long) irq4, 0x08, 0x8E);
  idt_set_gate(37, (unsigned long) irq5, 0x08, 0x8E);
  idt_set_gate(38, (unsigned long) irq6, 0x08, 0x8E);
  idt_set_gate(39, (unsigned long) irq7, 0x08, 0x8E);
  idt_set_gate(40, (unsigned long) irq8, 0x08, 0x8E);
  idt_set_gate(41, (unsigned long) irq9, 0x08, 0x8E);
  idt_set_gate(42, (unsigned long) irq10, 0x08, 0x8E);
  idt_set_gate(43, (unsigned long) irq11, 0x08, 0x8E);
  idt_set_gate(44, (unsigned long) irq12, 0x08, 0x8E);
  idt_set_gate(45, (unsigned long) irq13, 0x08, 0x8E);
  idt_set_gate(46, (unsigned long) irq14, 0x08, 0x8E);
  idt_set_gate(47, (unsigned long) irq15, 0x08, 0x8E);
}

void
irq_handler(registers_t r)
{
  void (*handler)(registers_t * r);

  handler = irq_routines[r.int_no - 32];

  if (handler && r.int_no - 32 != 0)
    handler(&r);

  if (r.int_no >= 40)
    serial_putc(SLAVE_COMMAND, 0x20);

  serial_putc(MASTER_COMMAND, 0x20);

  if (handler && r.int_no - 32 == 0)
    handler(&r);
}

void
irq_send_eoi(int irq)
{
  if (irq >= 8)
    serial_putc(SLAVE_COMMAND, 0x20);

  serial_putc(MASTER_COMMAND, 0x20);
}

void
irq_set_mask(unsigned char line)
{
  uint16_t port;
  uint8_t value;

  if(line < 8)
    port = MASTER_COMMAND;
  else
    {
      port = SLAVE_COMMAND;
      line -= 8;
    }

  value = serial_getc(port) | (1 << line);
  serial_putc(port, value);
}

void irq_clear_mask(unsigned char line)
{
  uint16_t port;
  uint8_t value;

  if(line < 8)
    port = MASTER_COMMAND;
  else
    {
      port = SLAVE_COMMAND;
      line -= 8;
    }

  value = serial_getc(port) & ~(1 << line);
  serial_putc(port, value);

}
