#include "stdio.h"
#include "io/vfb.h"
#include "kernel.h"
#include "string.h"
#include "multiboot.h"
#include "gdt.h"
#include "idt.h"
#include "irq.h"
#include "isr.h"
#include "kb.h"
#include "io/serial.h"

int kstart(unsigned long magic, struct multiboot_info * mbi)
{
    vfb_clear();

    if (magic != MULTIBOOT_MAGIC)
    {
        PANIC("Invalid magic multiboot");
        return (1);
    }

    printk("Splash %x!\n", mbi->flags);

    if (CHECK_FLAG(mbi->flags, 0))
      {
        printk("memory lower: %d\n", mbi->mem_lower);
        printk("memory upper: %d\n", mbi->mem_upper);
      }
    if (CHECK_FLAG(mbi->flags, 1))
      printk("boot device: %x\n", mbi->boot_device);
    if (CHECK_FLAG(mbi->flags, 2))
      printk("cmdline: %s\n", (char *) mbi->cmdline);

    printk("Initialization of serial port: ");
    serial_init(COM1, serial_bps_38400, "8N1");
    printk(SUCC_COLOR "ok\n" DEF_COLOR);

    printk("Initialization of GDTs: ");
    init_flat_gdt();
    printk(SUCC_COLOR "ok\n" DEF_COLOR);

    init_idt();

    printk("Initialization of IRQs: ");
    irq_init(0x20);
    printk(SUCC_COLOR "ok\n" DEF_COLOR);

    interrupts_init();

    asm volatile ("sti");

    printk("Initialization of KeyBoard: ");
    kb_init();
    printk(SUCC_COLOR "ok\n" DEF_COLOR);

    for (;;);

    return (0);
}
