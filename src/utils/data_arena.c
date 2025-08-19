#include <stdlib.h>
#include <string.h>

#include "data_arena.h"

TDataArena *get_data_arena()
{
    TDataArena *arena = (TDataArena *)malloc(sizeof(TDataArena));
    if (arena == NULL)
        return NULL;
    arena->start = (byte *)malloc(BASE_ARENA_SIZE);
    if (arena->start == NULL)
        return NULL;

    arena->last = arena->start;
    arena->capacity = BASE_ARENA_SIZE;
    return arena;
}

void *arena_malloc(TDataArena *arena, size_t size)
{
    size_t used = (size_t)(arena->last - arena->start);

    if (used + size > arena->capacity)
    {
        size_t new_capacity = arena->capacity * 2;
        if (new_capacity < used + size)
            new_capacity = used + size;

        byte *new_start = realloc(arena->start, new_capacity);
        if (!new_start)
            return NULL;

        arena->last = new_start + used;
        arena->start = new_start;
        arena->capacity = new_capacity;
    }
    void *ptr = arena->last;
    arena->last += size;
    return ptr;
}

// temp fast realization
void *arena_realloc(TDataArena *arena, void *block, size_t lastSize, size_t newSize)
{
    void *newBlock = arena_malloc(arena, newSize);
    if (newBlock == NULL)
        return NULL;
    memcpy(newBlock, block, lastSize);
}

void arena_reset(TDataArena *arena)
{
    arena->last = arena->start;
}

void arena_free(TDataArena *arena)
{
    free(arena->start);
    free(arena);
}
