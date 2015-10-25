ARCH=		i386
KERNEL=		jaundoeuf

CC=		gcc
RM=		rm
ASM=		nasm

CFLAGS	+=	-g -std=c99 -Wall -Werror -nostdlib -nostdinc
CFLAGS	+=	-fno-stack-protector -fno-builtin
CFLAGS	+=	-march=$(ARCH) -m32
CFLAGS	+= 	-I./include

AFLAGS	+=	-f elf
AFLAGS	+=	-I./include

LD=		ld
LDFLAGS	+=	-nostdlib -melf_$(ARCH)

LINKER	=	link.ld

ASMS=		$(wildcard src/*.s)
SRCS=		$(wildcard src/*.c)
OBJS=		$(SRCS:.c=.o) $(ASMS:.s=.o)

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

re: fclean all

qemu:
	qemu-system-i386 -kernel $(KERNEL)

.PHONY: all clean fclean re
