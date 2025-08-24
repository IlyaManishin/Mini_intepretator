#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "data_arena.h"

#define min(a, b) ((a) < (b) ? (a) : (b))

typedef struct TDataArena
{
    byte *start;
    byte *last;
    size_t capacity;
} TDataArena;

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
    assert(arena != NULL);

    size_t used = (size_t)(arena->last - arena->start);
    assert(used + size <= arena->capacity);
    // if (used + size > arena->capacity)
    // {
    //     size_t new_capacity = (used + size) * 2;

    //     byte *new_start = realloc(arena->start, new_capacity);
    //     if (!new_start)
    //         return NULL;

    //     arena->last = new_start + used;
    //     arena->start = new_start;
    //     arena->capacity = new_capacity;
    // }
    void *ptr = arena->last;
    arena->last += size;
    return ptr;
}

// temp fast realization
void *arena_realloc(TDataArena *arena, void *block, size_t lastSize, size_t newSize)
{
    assert(arena != NULL);

    void *newBlock = arena_malloc(arena, newSize);
    if (newBlock == NULL)
        return NULL;
    memcpy(newBlock, block, lastSize);
    return newBlock;
}

void arena_reset(TDataArena *arena)
{
    assert(arena != NULL);

    arena->last = arena->start;
}

void delete_arena(TDataArena *arena)
{
    if (arena == NULL)
        return;

    free(arena->start);
    free(arena);
}

void arena_free(TDataArena *arena, void *block)
{
    return;
}
