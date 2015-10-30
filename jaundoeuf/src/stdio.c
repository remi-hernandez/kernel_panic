#include "string.h"
#include "stdio.h"
#include "io/vfb.h"
#include "io/serial.h"

void
putc(char c)
{
    vfb_write(&c, 1);
    serial_putc(COM1, c);
}

void
puts(const char * s)
{
    vfb_write(s, strlen(s));

    for (int i = 0; i < strlen(s); ++i)
      serial_putc(COM1, s[i]);
}

void
putnbr(int n)
{
    if (n < 0)
    {
        putc('-');
        putnbr(n / -10);
        putc(-(n % 10) + '0');
    }
    else
    {
        if (n >= 10)
            putnbr(n / 10);

        putc((n % 10) + '0');
    }
}

void
putnbr_base(int n, const char * s)
{
    int l = strlen(s);

    if (n < 0)
    {
        putc('-');
        putnbr_base(n / -l, s);
        putc(s[-(n % l)]);
    }
    else
    {
        if (n >= l)
            putnbr_base(n / l, s);

        putc(s[(n % l)]);
    }
}
