#include "tss.h"
#include "string.h"
#include "gdt.h"

tss_t sys_tss;

extern void tss_flush();

void tss_set_kernel_stack(unsigned int stack)
{
    sys_tss.esp0 = stack;
}

void tss_init(signed int num, unsigned short ss0, unsigned short esp0)
{
    unsigned long int base = (unsigned long int) &sys_tss;
    size_t size = base + sizeof(tss_t);

    set_gdt_entry(num, base, size, 0xE9, 0x00);

    (void) memset((void *) &sys_tss, 0, sizeof(sys_tss));

    sys_tss.ss0 = ss0;
    sys_tss.esp0 = esp0;

    sys_tss.cs = 0x0B;
    sys_tss.ss = sys_tss.ds = sys_tss.es = sys_tss.fs = sys_tss.gs = 0x13;
}
