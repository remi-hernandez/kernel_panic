#include "string.h"
#include "types.h"

void *
memset(void * s, int c, size_t size)
{
    int     i;
    char *  tmp = s;

    for (i = 0; i < size; ++i)
        tmp[i] = c;

    return (s);
}

int
strlen(const char * s)
{
    int     i = 0;

    while (s[i])
        ++i;

    return (i);
}

int
strcmp(const char * s1, const char * s2)
{
    int     i = 0;

    while (s1[i] && s2[i] && s1[i] == s2[i])
        ++i;

    return (s1[i] - s2[i]);
}
