#include <assert.h>
#include <stdio.h>
#include <string.h>

#include <parser_errors.h>
#include <system_tools.h>

static void print_error_line(const char *errLine, const char *end)
{
    while (*errLine != '\n' &&
           *errLine != '\0' &&
           errLine < end)
    {
        printf("%c", *errLine++);
    }
    printf("\n");
}

void print_error_msg(char *textMsg)
{
    printf("%s:\n", textMsg);
}

void print_error_with_pos(char *textMsg, TErrorBufferPos pos)
{
    print_error_msg(textMsg);

    int line_pointer_offset = printf("%d line: ", pos.errLineIndex + 1);
    print_error_line(pos.errLineStart, pos.bufferEnd);
    int pointer_offset = line_pointer_offset + (pos.errPos - pos.errLineStart);
    printf("%*c^\n\n", pointer_offset, ' ');
}

TErrorBufferPos get_buffer_error_pos(char *errPos, char *errLineStart, int errLineIndex, char *bufferEnd)
{
    TErrorBufferPos pos;
    pos.errPos = errPos;
    pos.errLineStart = errLineStart;
    pos.errLineIndex = errLineIndex;
    pos.bufferEnd = bufferEnd;

    return pos;
}
