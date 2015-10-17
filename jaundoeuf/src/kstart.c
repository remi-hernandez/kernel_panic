#include "stdio.h"
#include "kernel.h"

int kstart(int code)
{
    if (code != MULTIBOOT_MAGIC)
    {
        PANIC("Invalid magic multiboot");
    }

    printk("Splash!\n");

    return (0);
}
