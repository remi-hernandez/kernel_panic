#ifndef __KERNEL__
#define __KERNEL__

#include "stdio.h"

#ifndef NULL
#define NULL 0
#endif

#define VIRTUAL_BASE 0xC0000000
#define MULTIBOOT_MAGIC 0x2BADB002
#define PANIC(msg) do { puts(ERR msg); while (1); } while (0);

#endif
