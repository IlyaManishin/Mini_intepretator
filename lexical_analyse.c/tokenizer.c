#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tokenizer.h"

#define TOKENS_BUF_SIZE 16

void delete_tokenizer(TTokenizer *tokenizer)
{
    //!!!
    free(tokenizer);
}

TTokenizer *tokenizer_from_file(FILE *file)
{
    if (file == NULL)
    {
        return NULL;
    }
    fseek(file, 0, SEEK_END);
    size_t bufferSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *buf = (char *)malloc(bufferSize);
    if (buf == NULL)
        return NULL;
    fread(buf, 1, bufferSize, file);

    TTokenizer *tokenizer = (TTokenizer *)malloc(sizeof(TTokenizer));
    if (tokenizer == NULL)
        return NULL;

    tokenizer->buf = buf;
    tokenizer->cur = buf;
    tokenizer->curLine = buf;
    tokenizer->end = buf + bufferSize - 1;

    tokenizer->state = NEW_LINE_STATE;
    tokenizer->curIndent = 0;

    tokenizer->tokensBuf = (TToken *)malloc(TOKENS_BUF_SIZE * sizeof(TToken));
    if (tokenizer->tokensBuf == NULL)
    {
        goto invalid_tokens_buf;
    }
    return tokenizer;

invalid_tokens_buf:
    free(tokenizer->buf);
    free(tokenizer);

    return NULL;
}

int tgetc(TTokenizer *tokenizer, char *ch)
{
    if (tokenizer->cur > tokenizer->end)
    {
        return EOF;
    }
    *ch = *tokenizer->cur;
    tokenizer->cur++;
    return 0;
}

void tbackc(TTokenizer *tokenizer, char ch)
{
    tokenizer->cur --;
    assert(*tokenizer->cur == ch);
}

int _get_new_line_indent(TTokenizer *tokenizer)
{
    int tabCnt = 0;
    int whiteSpaceCnt = 0;
}

int _read_token(TTokenizer *tokenizer, TToken *tokDest)
{
    if (tokenizer->state == NEW_LINE_STATE)
    {
        tokenizer->curIndent = _get_new_line_indent(tokenizer);
        assert(tokenizer->curIndent != -1);
    }
}
