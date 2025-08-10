#pragma once
#include <stdlib.h>

#define BASE_ARENA_SIZE 1024

typedef char byte;

typedef struct TDataArena
{
    byte *start;
    byte *last;
    size_t capacity;
} TDataArena;

TDataArena *get_data_arena();
void *arena_malloc(TDataArena *arena, size_t size);
void arena_reset(TDataArena *arena);
void arena_free(TDataArena *arena);