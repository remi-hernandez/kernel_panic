#ifndef __SERIAL__
#define __SERIAL__

#include "types.h"

#define COM1 0x3F8
typedef enum {serial_bps_38400} serial_bps_t;

void  outb(dword out, dword value);
dword inb(dword port);

void  serial_init(int serial_port, serial_bps_t serial_speed, char *config);
int   serial_received(int port);
int   serial_getc(int port);
void  serial_putc(int port, int c);

#endif
