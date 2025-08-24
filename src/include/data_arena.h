#pragma once
#include <stdlib.h>

// #define BASE_ARENA_SIZE 1024
#define BASE_ARENA_SIZE 1024000 // to delete realloc


typedef char byte;
typedef struct TDataArena TDataArena;

TDataArena *get_data_arena();
void *arena_malloc(TDataArena *arena, size_t size);
void *arena_realloc(TDataArena *arena, void *block, size_t lastSize, size_t newSize);
void arena_reset(TDataArena *arena);
void arena_free(TDataArena *arena, void *block);
void delete_arena(TDataArena *arena);
