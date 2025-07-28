#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/tokenizer_api.h"
#include "tokenizer.h"

#define get_tokenizer_error(tokenizer, textMsg) \
    get_error(textMsg, tokenizer->curLine, tokenizer->curLineIndex, tokenizer->cur)

#define is_pass_symbol(ch) ((ch) == '\r' || (ch) == '\t' || (ch) == ' ')
#define is_ident_start_symbol(ch) (((ch) > 'a' && (ch) < 'z') || ((ch) > 'A' && (ch) < 'Z') || (ch) == '_')

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
    tokenizer->curLineIndex = 0;
    tokenizer->end = buf + bufferSize - 1;

    tokenizer->state = NEW_LINE_STATE;
    tokenizer->newIndent = 0;
    tokenizer->curIndent = 0;

    tokenizer->tokensBuf = (TToken *)malloc(TOKENS_BUF_SIZE * sizeof(TToken));
    if (tokenizer->tokensBuf == NULL)
    {
        goto invalid_tokens_buf;
    }

    tokenizer->isError = false;
    tokenizer->errorType = NONE;
    tokenizer->errMesg.textMsg = NULL;

    return tokenizer;

invalid_tokens_buf:
    free(tokenizer->buf);
    free(tokenizer);

    return NULL;
}

static bool is_tokenizer_error(TTokenizer *tokenizer)
{
    return tokenizer->isError;
}

static int tgetc(TTokenizer *tokenizer, char *ch)
{
    if (tokenizer->cur > tokenizer->end)
    {
        return EOF;
    }
    *ch = *tokenizer->cur;
    tokenizer->cur++;
    return 0;
}

static void tbackc(TTokenizer *tokenizer, char ch)
{
    tokenizer->cur--;
    assert(*tokenizer->cur == ch);
}

static TToken make_empty_token()
{
    TToken token;
    token.type = INVALID;
    token.start = NULL;
    token.end = NULL;

    return token;
}

static TToken make_keyword_token(TTokenizer *tokenizer, TokenTypes type)
{
    TToken token;
    token.type = type;
    token.start = tokenizer->cur;
    token.end = tokenizer->cur;

    return token;
}

static int _get_new_line_indent(TTokenizer *tokenizer)
{
    int tabCount = 0;
    int whiteSpaceCount = 0;
    char ch;
    while (tgetc(tokenizer, &ch) != EOF)
    {
        if (ch == '\n') // empty line
        {
            return tokenizer->newIndent;
        }
        if (ch == ' ')
        {
            whiteSpaceCount++;
            if (whiteSpaceCount == WHITESPACE_IN_TAB)
            {
                whiteSpaceCount = 0;
                tabCount++;
            }
        }
        else if (ch == '\t')
        {
            tabCount++;
        }
        else
        {
            tbackc(tokenizer, ch);
            if (whiteSpaceCount != 0)
            {
                tokenizer->errMesg = get_tokenizer_error(tokenizer, "Invalid code intend");
                return -1;
            }
        }
    }
}

static TToken read_token(TTokenizer *tokenizer)
{
    if (tokenizer->state == NEW_LINE_STATE)
    {
        tokenizer->newIndent = _get_new_line_indent(tokenizer);
        if (is_tokenizer_error(tokenizer))
            return make_empty_token();
        tokenizer->state = INSIDE_LINE_STATE;
    }

    if (tokenizer->newIndent < tokenizer->curIndent)
    {
        tokenizer->curIndent--;
        return make_keyword_token(tokenizer, DEDENT);
    }
    else if (tokenizer->newIndent > tokenizer->curIndent)
    {
        tokenizer->curIndent++;
        return make_keyword_token(tokenizer, INDENT);
    }

    char ch = '\0';
    while (tgetc(tokenizer, &ch) != EOF)
    {
        if (is_pass_symbol(ch))
            continue;
    }
    if (ch == '\0')
        return make_keyword_token(tokenizer, EOF_TOKEN);
    
    
}
