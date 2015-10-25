#include "stdio.h"
#include "io/vfb.h"
#include "kernel.h"
#include "string.h"

int kstart(int code)
{
    // Clear output
    memset(VID_FRAME_BUF, 0, NB_LINE * NB_COLUMN);

    if (code != MULTIBOOT_MAGIC)
    {
        PANIC("Invalid magic multiboot");
        return (1);
    }

    printk("Splash %x!\n", code);

    return (0);
}
