#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/tokenizer_api.h"
#include "tokenizer.h"

#define get_tokenizer_error(tokenizer, textMsg) \
    get_error(textMsg, tokenizer->curLine, tokenizer->curLineIndex, tokenizer->start)

#define is_pass_symbol(ch) ((ch) == '\r' || (ch) == '\t' || (ch) == ' ')
#define is_ident_or_kw_start_symbol(ch) (((ch) > 'a' && (ch) < 'z') || ((ch) > 'A' && (ch) < 'Z') || (ch) == '_')
#define is_digit(ch) ((ch) >= '0' && (ch) <= '9')


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

    char *buf = (char *)malloc(bufferSize + 1);
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
    tokenizer->end = buf + bufferSize;

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
    if (tokenizer->cur >= tokenizer->end)
    {
        *ch = '\0';
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

static void tgets(TTokenizer *tokenizer, char *s)
{
    char ch;
    while (*s != '\0')
    {
        tgetc(tokenizer, &ch);
        assert(ch == *s);
        s++;
    }
}

static bool lookahead(TTokenizer *tokenizer, char *check)
{
    bool result = true;

    char *cur = check;
    char tok_ch;
    while (*cur != '\0')
    {
        int r = tgetc(tokenizer, &tok_ch);
        if (r == EOF || tok_ch != *cur)
        {
            result = false;
            cur--;
            if (r != EOF)
                tbackc(tokenizer, tok_ch);
            break;
        }
        cur++;
    }
    tgetc(tokenizer, &tok_ch);

    while (cur >= check)
    {
        tbackc(tokenizer, *cur);
        cur--;
    }
    return result;
}

static TToken make_empty_token()
{
    TToken token;
    token.type = INVALID;
    token.start = NULL;
    token.end = NULL;

    return token;
}

static TToken make_token(TTokenizer *tokenizer, TokenTypes type)
{
    TToken token;
    token.type = type;
    token.start = tokenizer->start;
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
            return tokenizer->curIndent;
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
                tokenizer->start = tokenizer->cur;
                tokenizer->errMesg = get_tokenizer_error(tokenizer, "Invalid code intend");
                return -1;
            }
        }
    }
    return tabCount;
}

static bool try_to_tgets(TTokenizer *tokenizer, char *s)
{
    bool isValid = false;

    if (lookahead(tokenizer, s))
    {
        tgets(tokenizer, s);
        isValid = true;
    }
    return isValid;
}
static TToken read_keyword_token(TTokenizer *tokenizer)
{
    char first;
    tgetc(tokenizer, &first);

    TokenTypes type = INVALID;

    switch (first)
    {
    case 'r':
        if (try_to_tgets(tokenizer, "eturn"))
            type = RETURN_KW;
        break;
    case 'i':
        if (try_to_tgets(tokenizer, "f"))
            type = IF_KW;
        else if (try_to_tgets(tokenizer, "mport"))
            type = IMPORT_KW;
        break;
    case 'e':
        if (try_to_tgets(tokenizer, "lse"))
            type = ELSE_KW;
        else if (try_to_tgets(tokenizer, "lif"))
            type = ELIF_KW;
        break;
    case 'f':
        if (try_to_tgets(tokenizer, "or"))
            type = FOR_KW;
        else if (try_to_tgets(tokenizer, "unc"))
            type = FUNC_KW;
        else if (try_to_tgets(tokenizer, "rom"))
            type = FROM_KW;
        break;
    case 'b':
        if (try_to_tgets(tokenizer, "reak"))
            type = BREAK_KW;
        break;
    case 'c':
        if (try_to_tgets(tokenizer, "ontinue"))
            type = CONTINUE_KW;
        break;
    case 'a':
        if (try_to_tgets(tokenizer, "nd"))
            type = AND_KW;
        break;
    case 'o':
        if (try_to_tgets(tokenizer, "r"))
            type = OR_KW;
        break;
    case 'n':
        if (try_to_tgets(tokenizer, "ot"))
            type = NOT_KW;
        break;
    case 'w':
        if (try_to_tgets(tokenizer, "hile"))
            type = WHILE_KW;
        break;
    case 'p':
        if (try_to_tgets(tokenizer, "ass"))
            type = PASS_KW;
        break;
    case 'T':
        if (try_to_tgets(tokenizer, "rue"))
            type = TRUE_KW;
        break;
    case 'F':
        if (try_to_tgets(tokenizer, "alse"))
            type = FALSE_KW;
        break;
    case 'N':
        if (try_to_tgets(tokenizer, "ull"))
            type = NULL_KW;
        break;
    }

    if (type == INVALID)
        tbackc(tokenizer, first);

    TToken result = make_token(tokenizer, type);
    return result;
}

static TToken read_ident_token(TTokenizer *tokenizer)
{
    char ch;
    do
    {
        tgetc(tokenizer, &ch);
    } while (is_ident_or_kw_start_symbol(ch) || is_digit(ch));

    return make_token(tokenizer, IDENT);
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

    tokenizer->start = tokenizer->cur;
    if (tokenizer->newIndent < tokenizer->curIndent)
    {
        tokenizer->curIndent--;
        return make_token(tokenizer, DEDENT);
    }
    else if (tokenizer->newIndent > tokenizer->curIndent)
    {
        tokenizer->curIndent++;
        return make_token(tokenizer, INDENT);
    }

    char ch = '\0';
    while (tgetc(tokenizer, &ch) != EOF)
    {
        if (!is_pass_symbol(ch))
            break;
    }
    if (ch == '\0')
        return make_token(tokenizer, EOF_TOKEN);
    tbackc(tokenizer, ch);

    tokenizer->start = tokenizer->cur;

    if (is_ident_or_kw_start_symbol(ch))
    {
        TToken keywordToken = read_keyword_token(tokenizer);
        if (keywordToken.type != INVALID)
        {
            return keywordToken;
        }
        TToken identToken = read_ident_token(tokenizer);
        return identToken;
    }
}
