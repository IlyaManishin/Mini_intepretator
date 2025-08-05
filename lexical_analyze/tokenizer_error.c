#include <assert.h>
#include <stdio.h>
#include <string.h>

#include <system_tools.h>
#include <tokenizer_api.h>

TTokenizerError make_pos_error(char *textMsg,char *errLine, int lineIndex, char *cur, char *end)
{
    TTokenizerError error;
    error.textMsg = textMsg;

    error.withPos = true;
    error.errLine = errLine;
    error.errLineIndex = lineIndex;
    error.errColumn = cur - errLine;
    error.end = end;
    return error;
}

TTokenizerError make_base_error(char *textMsg)
{
    TTokenizerError error;
    error.textMsg = textMsg;
    error.withPos = false;

    return error;
}

static void print_error_line(const char *errLine, const char* end)
{
    while (*errLine != '\n' &&
           *errLine != '\0' &&
           errLine < end)
    {
        printf("%c", *errLine++);
    }
    printf("\n");
}

void tokenizer_print_error(TTokenizer *tokenizer)
{
    assert(tokenizer->isError && tokenizer->errMesg.textMsg != NULL);

    TTokenizerError error = tokenizer->errMesg;
    printf("%s:\n", error.textMsg);

    if (!error.withPos)
        return;

    int line_pointer_offset = printf("%d line: ", error.errLineIndex + 1);
    print_error_line(error.errLine, error.end);
    int pointer_offset = line_pointer_offset + error.errColumn;
    printf("%*c^\n\n", pointer_offset, ' ');
}