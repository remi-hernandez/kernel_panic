#include "gdt.h"
#include "stdio.h"
#include "tss.h"

extern void gdt_flush(gdt_ptr_t * gdt_ptr);

gdt_entry_t gdt_entries[5];
gdt_ptr_t   gdt_ptr;


void
init_flat_gdt()
{
  gdt_ptr.limit = (sizeof(gdt_entry_t) * 5) - 1;
  gdt_ptr.base = (unsigned long) &gdt_entries;

  /*
                        Pr  Priv  1   Ex  DC   RW   Ac
  0x9A == 1001 1010  == 1   00    1   1   0    1    0
  0x92 == 1001 0010  == 1   00    1   0   0    1    0
  0xFA == 1111 1010  == 1   11    1   1   0    1    0
  0xF2 == 1111 0010  == 1   11    1   0   0    1    0

  We have page-granularity and 32-bit mode

                        G   D   0   Av
  0xCF == 1100 1111  == 1   1   0   0  ~
  */

  set_gdt_entry(0,0,0,0,0);                    // Null segment
  set_gdt_entry(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // Code segment
  set_gdt_entry(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // Data segment
  set_gdt_entry(3, 0, 0xFFFFFFFF, 0xFA, 0xCF); // User mode code segment
  set_gdt_entry(4, 0, 0xFFFFFFFF, 0xF2, 0xCF); // User mode data segment

  gdt_flush(&gdt_ptr);
}

void
set_gdt_entry(int32_t entry, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran)
{
  gdt_entries[entry].base_low = (base & 0xFFFF);
  gdt_entries[entry].base_middle = (base >> 16) & 0xFF;
  gdt_entries[entry].base_high = (base >> 24) & 0xFF;

  gdt_entries[entry].limit_low = (limit & 0xFFFF);
  gdt_entries[entry].granularity = (limit >> 16) & 0x0F;

  gdt_entries[entry].granularity |= gran & 0xF0;
  gdt_entries[entry].access = access;
}
