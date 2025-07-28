#pragma once

#include "../include/tokenizer_api.h"

typedef enum TokenizerSettings
{
    TOKENS_BUF_SIZE = 16,
    WHITESPACE_IN_TAB = 4,
} TokenizerSettings;

TTokenizerError get_error(char* textMsg, char* errLine, int lineIndex, char *cur);