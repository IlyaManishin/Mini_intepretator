#include <assert.h>
#include <stdio.h>
#include <string.h>

#include <parser_errors.h>
#include <system_tools.h>

#include "tokenizer_api.h"

TTokenizerError make_pos_error_by_cur(char *textMsg, char *errLine, int lineIndex, char *cur, char *end)
{
    TTokenizerError error;
    error.textMsg = textMsg;

    error.withPos = true;
    error.pos = get_buffer_error_pos(errLine, cur, lineIndex, end);
    return error;
}

TTokenizerError make_base_error(char *textMsg)
{
    TTokenizerError error;
    error.textMsg = textMsg;
    error.withPos = false;

    return error;
}


