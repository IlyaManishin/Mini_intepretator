#pragma once

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "parser_errors.h"
#include "system_tools.h"
#include "token.h"

typedef enum TokenizerStates
{
    NEW_LINE_STATE,
    INSIDE_LINE_STATE,
    EOF_STATE,
} TokenizerStates;

typedef struct TTokenBuffer TTokenBuffer;

typedef struct TTokenizer
{
    const char *start;
    const char *cur;
    const char *curLine; // start of code or after \n
    int lineIndex;
    const char *end;

    TokenizerStates state;
    int curIndent;
    int newIndent;

    TTokenBuffer *tokensBuf;

    bool isError;
    TTokenizerError tokError;
} TTokenizer;

TTokenizer *tokenizer_from_file_data(TFileData fileData);
void delete_tokenizer(TTokenizer *tokenizer);
TToken read_new_token(TTokenizer *tokenizer);

bool is_tokenizer_error(TTokenizer *tokenizer);
TTokenizerError get_tokenizer_error(TTokenizer *tokenizer);
void pass_tokenizer_error(TTokenizer *tokenizer);

TToken soft_token_read(TTokenizer *tokenizer);
TToken strong_token_read(TTokenizer *tokenizer);