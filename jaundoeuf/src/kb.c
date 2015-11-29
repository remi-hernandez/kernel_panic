#include "irq.h"
#include "kb.h"
#include "registers.h"
#include "stdio.h"
#include "io/serial.h"
#include "irq.h"

#define KB_SHIFT 0x11
#define KB_ALT   0x12
#define KB_CTRL  0x13
#define KBD_LAYOUT kbd_us

#include <kbd/us.h>

unsigned short keystatus = 0x0000;
int            last_input;

void
keyboard_handler(registers_t * r)
{
  unsigned char scancode;

  scancode = serial_getc(0x60);

  if (scancode & 0x80)
    {
      if (KBD_LAYOUT[scancode] == KB_SHIFT)
          keystatus &= ~0x0100;
      else if (KBD_LAYOUT[scancode] == KB_CTRL)
          keystatus &= ~0x0010;
      else if (KBD_LAYOUT[scancode] == KB_ALT)
          keystatus &= ~0x0001;
    }
  else
    {
      if (KBD_LAYOUT[scancode] == KB_SHIFT)
          keystatus |= 0x0100;
      else if (KBD_LAYOUT[scancode] == KB_CTRL)
          keystatus |= 0x0010;
      else if (KBD_LAYOUT[scancode] == KB_ALT)
          keystatus |= 0x0001;

      if ((keystatus & 0x0100) == 0x0100)
        {
          putc(KBD_LAYOUT[scancode+128]);
          last_input = scancode + 128;
        }
      else
        {
          putc(KBD_LAYOUT[scancode]);
          last_input = scancode;
        }
    }
}

#undef KBD_LAYOUT

void
kb_init()
{
  irq_install_handler(1, keyboard_handler);
}

int
kb_get_key()
{
  return (last_input);
}
