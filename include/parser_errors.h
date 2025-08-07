#pragma once

typedef struct TErrorBufferPos
{
    char *errPos;
    char *errLineStart;
    int errLineIndex;
    char *bufferEnd;
} TErrorBufferPos;

void print_error_msg(char *textMsg);
void print_error_with_pos(char *errMsg, TErrorBufferPos errorPosInfo);
TErrorBufferPos get_buffer_error_pos(char *errPos, char *errLineStart, int errLineIndex, char *bufferEnd);