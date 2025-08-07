#pragma once

#include "tokenizer_api.h"

typedef enum TokenizerSettings
{
    TOKENS_BUF_SIZE = 16,
    WHITESPACE_IN_TAB = 4,

    MAX_IDENT_LENGTH = 128
} TokenizerSettings;
