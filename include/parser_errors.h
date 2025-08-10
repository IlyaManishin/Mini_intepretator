#pragma once

#include <stdbool.h>

typedef struct TErrorBufferPos
{
    const char *errPos;
    const char *errLineStart;
    int errLineIndex;
    const char *bufferEnd;
} TErrorBufferPos;

typedef struct TTokenizerError
{
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
TErrorBufferPos get_buffer_error_pos(const char *errPos, const char *errLineStart, int errLineIndex, const char *bufferEnd);