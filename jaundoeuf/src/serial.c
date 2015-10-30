#include "io/serial.h"

#define BUFFER_SIZE 256

void serial_init(int serial_port, serial_bps_t serial_speed, char *config)
{
  outb(serial_port + 1, 0x00); // Disable all interrupts
  outb(serial_port + 3, 0x80); // Enable DLAB (set baud rate divisor)
  outb(serial_port + 0, 0x03); // Set divisor to 3 (lo byte) 38400 baud
  outb(serial_port + 1, 0x00); //                  (hi byte)
  outb(serial_port + 3, 0x03); // 8 bits, no parity, one stop bit
  outb(serial_port + 2, 0xC7); // Enable FIFO, clear them, with 14-byte threshold
  outb(serial_port + 4, 0x0B); // IRQs enabled, RTS/DSR set
  outb(serial_port + 1, 0x01); // Enable interrups
}

int serial_received(int port)
{
  return (inb(port + 5) & 1);
}

int serial_getc(int port)
{
  if (!serial_received(port))
    return (-1);
  else
    return (inb(port));
}

void serial_putc(int port, int value)
{
  outb(port, value);
}
