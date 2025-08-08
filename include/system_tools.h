#pragma once

#include <stdlib.h>
#include <stdio.h>

typedef struct TFileData
{
    const char* str;
    size_t dataSize;
} TFileData;

size_t nsu_strnlen(const char *s, size_t maxlen);

TFileData read_file_data(FILE *file);
void delete_file_data(TFileData data);
