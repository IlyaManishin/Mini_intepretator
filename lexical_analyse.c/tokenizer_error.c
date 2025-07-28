#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "../include/system_tools.h"
#include "../include/tokenizer_api.h"

TTokenizerError get_error(char *textMsg, char *errLine, int lineIndex, char *cur)
{
    TTokenizerError error;
    error.textMsg = textMsg;
    error.errLine = errLine;
    error.errLineIndex = lineIndex;
    error.errColumn = cur - errLine;

    return error;
}

void tokenizer_print_error(TTokenizer *tokenizer)
{
    assert(tokenizer->isError && tokenizer->errMesg.textMsg != NULL);

    TTokenizerError error = tokenizer->errMesg;
    printf("%s:\n", error.textMsg);

    int line_pointer_offset = printf("%d line: ", error.errLineIndex + 1);

    int pointer_offset = line_pointer_offset + error.errColumn;
    printf("%*c^\n", pointer_offset, ' ');
}