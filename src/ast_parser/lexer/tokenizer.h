#pragma once

#include "tokenizer_api.h"

typedef enum TokenizerSettings
{
    TOKENS_BUF_SIZE = 16,
    WHITESPACE_IN_TAB = 4,

    MAX_IDENT_LENGTH = 128
} TokenizerSettings;

TToken read_new_token(TTokenizer *tokenizer);
TToken make_error_token(TTokenizer *tokenizer);
void set_memory_error(TTokenizer *tokenizer);