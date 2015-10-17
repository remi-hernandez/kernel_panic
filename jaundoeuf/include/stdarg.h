#ifndef __STDARG__
#define __STDARG__

typedef char * va_list;

#define va_start(l, p) \
  (l = (va_list)((char *) &p + sizeof(p)))
#define va_arg(l, t) \
  (*(t *)((l += sizeof(t)) - sizeof(t)))

#endif
