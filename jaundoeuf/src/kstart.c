#include "stdio.h"
#include "io/vfb.h"
#include "kernel.h"
#include "string.h"
#include "io/serial.h"

int kstart(int code)
{
    // int  c;

    // Clear output
    memset(VID_FRAME_BUF, 0, NB_LINE * NB_COLUMN);

    if (code != MULTIBOOT_MAGIC)
    {
        PANIC("Invalid magic multiboot");
        return (1);
    }

    serial_init(COM1, serial_bps_38400, "8N1");

    printk("Splash!\n", code);
    printk("Initialization of serial port: ");
    printk(SUCC_COLOR "ok\n" DEF_COLOR);

    /*
    while (1)
    {
       if ((c = serial_getc(COM1)) != -1)
       {
         printk("< %d\n", c);
         serial_putc(COM1, c);
         printk("> %d\n", c);
       }
    }
    */

    return (0);
}
