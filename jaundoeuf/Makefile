ARCH=      i386
KERNEL=    jaundoeuf.bin
ISO=       $(KERNEL:.bin=.iso)

CC=        gcc
RM=        rm
ASM=       nasm

CFLAGS  += -g -Wall -nostdlib -nostdinc
CFLAGS  += -fno-stack-protector -fno-builtin
CFLAGS  += -march=$(ARCH) -m32
CFLAGS  += -I./include

AFLAGS  += -f elf
AFLAGS  += -I./include

LD=        ld
LDFLAGS += -nostdlib -melf_$(ARCH)

LINKER=    kfs.ld

ASMS=      $(wildcard src/*.s)
SRCS=      $(wildcard src/*.c)
OBJS=      $(SRCS:.c=.o) $(ASMS:.s=.o)

all: $(KERNEL)

$(KERNEL): $(OBJS) $(LINKER)
	$(LD) $(LDFLAGS) -static -o $(KERNEL) $(OBJS) -T $(LINKER)

%.o: %.c
	$(CC) $(CFLAGS) -c $^ -o $@

%.o: %.s
	$(ASM) $(AFLAGS) -o $@ $^

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(KERNEL)
	$(RM) -rf isodir
	$(RM) *.iso

re: fclean all

boot:
	qemu-system-i386 -serial stdio -kernel $(KERNEL)

debug:
	qemu-system-i386 -serial stdio -kernel $(KERNEL) -s -S &
	gdb jaundoeuf

iso: $(ISO)

isodir isodir/boot isodir/boot/grub:
	mkdir -p $@

isodir/boot/$(KERNEL): $(KERNEL) isodir/boot
	cp $< $@

isodir/boot/grub/grub.cfg: grub.cfg isodir/boot/grub
	cp $< $@

$(ISO): isodir/boot/$(KERNEL) isodir/boot/grub/grub.cfg
	grub-mkrescue -o $@ isodir

run-qemu: $(ISO)
	qemu-system-i386 -cdrom $(ISO) -monitor stdio -d cpu_reset

.PHONY: all clean fclean re
