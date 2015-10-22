#include "stdio.h"
#include "kernel.h"

int kstart(int code)
{
    if (code != MULTIBOOT_MAGIC)
    {
        PANIC("Invalid magic multiboot");
        return (1);
    }

    printk("Splash!\n", code);

    return (0);
}
