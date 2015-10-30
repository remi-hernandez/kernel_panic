#ifndef __STDIO__
#define __STDIO__

#define ERR_COLOR "\x1B[31m"
#define WARN_COLOR "\x1B[33m"
#define INFO_COLOR "\x1B[36m"
#define SUCC_COLOR "\x1B[32m"
#define DEF_COLOR "\x1B[37m"
#define HEX_BASE "0123456789ABCDEF"

void putc(char c);
void puts(const char * s);
void putnbr(int n);
void putnbr_base(int n, const char * b);
void putunbr(unsigned int n);
void putunbr_base(unsigned int n, const char * b);
void printk(char const * s, ...);

#endif
