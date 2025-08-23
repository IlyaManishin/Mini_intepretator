#include <stdlib.h>

size_t nsu_strnlen(const char *s, size_t maxlen)
{
    size_t i = 0;
    while (i < maxlen && s[i] != '\0')
    i++;
    return i;
}

