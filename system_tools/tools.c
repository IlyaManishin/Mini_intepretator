#include "../include/system_tools.h"

#if defined(_WIN32)
size_t nsu_strnlen(const char *s, size_t maxlen) {
    size_t i = 0;
    while (i < maxlen && s[i] != '\0') i++;
    return i;
}
#else
#define _POSIX_C_SOURCE 200809L
#include <string.h>

size_t nsu_strnlen(const char *s, size_t maxlen) {
    return strnlen(s, maxlen);
}
#endif
