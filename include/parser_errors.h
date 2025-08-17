#pragma once

#include <stdbool.h>

typedef struct TErrorBufferPos
{
    const char *errStart;
    const char *errLineStart;
    int errLineIndex;
    const char *bufferEnd;
} TErrorBufferPos;

typedef enum TokenizerErrTypes
{
    INVALID_TOKEN,
    MEMORY_ERROR
} TokenizerErrTypes;

typedef struct TTokenizerError
{
    TokenizerErrTypes type;
    const char *textMsg;

    bool withPos;
    TErrorBufferPos pos;
} TTokenizerError;

typedef struct TAstParserError
{
    char *textMsg;
    TErrorBufferPos pos;
    //???
} TAstParserError;

void print_error_msg(const char *textMsg);
void print_error_with_pos(const char *errMsg, TErrorBufferPos errorPosInfo);
TErrorBufferPos get_buffer_error_pos(const char *errStart, const char *errLineStart, int errLineIndex, const char *bufferEnd);