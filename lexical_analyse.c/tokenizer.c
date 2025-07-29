#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/tokenizer_api.h"
#include "tokenizer.h"

// #define get_tokenizer_err(tokenizer, textMsg) \
//     make_error(textMsg, tokenizer->curLine, tokenizer->curLineIndex, tokenizer->cur)

#define is_pass_symbol(ch) ((ch) == '\r' || (ch) == '\t' || (ch) == ' ')
#define is_ident_or_kw_start_symbol(ch) (((ch) > 'a' && (ch) < 'z') || ((ch) > 'A' && (ch) < 'Z') || (ch) == '_')
#define is_digit(ch) ((ch) >= '0' && (ch) <= '9')
#define is_string_start_symbol(ch) (ch == '"' || ch == '\'' || ch == '`')

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

bool is_tokenizer_error(TTokenizer *tokenizer)
{
    return tokenizer->isError;
}

static void set_tokenizer_error(TTokenizer *tokenizer, char *errPos, char *textMsg)
{
    tokenizer->errMesg = make_error(textMsg, tokenizer->curLine, tokenizer->curLineIndex, errPos);
    tokenizer->isError = true;
}

TTokenizerError get_tokenizer_error(TTokenizer *tokenizer)
{
    return tokenizer->errMesg;
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

static bool tcheckc(TTokenizer *tokenizer, char check)
{
    char ch;
    int r = tgetc(tokenizer, &ch);
    if (r == EOF || ch != check)
    {
        return false;
    }
    return true;
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
    token.type = EOF_TOKEN;
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

static int read_new_line_indent(TTokenizer *tokenizer)
{
    int whiteSpaceCount = 0;
    char ch;
    while (tgetc(tokenizer, &ch) != EOF)
    {
        if (ch == ' ')
        {
            whiteSpaceCount++;
        }
        else if (ch == '\t')
        {
            whiteSpaceCount += WHITESPACE_IN_TAB;
        }
        else
        {
            tbackc(tokenizer, ch);
            break;
        }
    }
    return whiteSpaceCount;
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
    bool isEOF = false;
    do
    {
        int r = tgetc(tokenizer, &ch);
        if (r == EOF)
        {
            isEOF = true;
            break;
        }
    } while (is_ident_or_kw_start_symbol(ch) || is_digit(ch));
    if (!isEOF)
        tbackc(tokenizer, ch);
    return make_token(tokenizer, IDENT);
}

static int _read_digits_part(TTokenizer *tokenizer)
{
    int count;
    char ch;
    while (tgetc(tokenizer, &ch) != EOF)
    {
        if (ch == '_')
            continue;
        if (!is_digit(ch))
        {
            tbackc(tokenizer, ch);
            break;
        }
        count++;
    }
    return count;
}

static TToken read_number(TTokenizer *tokenizer)
{
    _read_digits_part(tokenizer);
    char after;
    int r = tgetc(tokenizer, &after);
    if (r != EOF && after == '.')
    {
        int digits_count = _read_digits_part(tokenizer);
        if (digits_count == 0)
        {
            set_tokenizer_error(tokenizer, tokenizer->cur, "Invalid number fraction");
            return make_token(tokenizer, INVALID);
        }
        r = tgetc(tokenizer, &after);
    }
    if (r == EOF)
        return make_token(tokenizer, NUMBER);

    tbackc(tokenizer, after);
    if (is_ident_or_kw_start_symbol(after))
    {
        set_tokenizer_error(tokenizer, tokenizer->cur, "Unexpected symbol after number");
        return make_token(tokenizer, INVALID);
    }
    return make_token(tokenizer, NUMBER);
}

static TToken read_string_token(TTokenizer *tokenizer, char leftEdge)
{
    char ch;
    while (tgetc(tokenizer, &ch) != EOF)
    {
        if (ch == leftEdge)
        {
            tbackc(tokenizer, ch);
            TToken stringToken = make_token(tokenizer, STRING);
            tgetc(tokenizer, &ch);
            return stringToken;
        }
        else if (is_string_start_symbol(ch))
        {
            set_tokenizer_error(tokenizer, tokenizer->cur, "Invalid symbol inside string");
            return make_token(tokenizer, INVALID);
        }
    }
}

static TToken read_token(TTokenizer *tokenizer)
{
    int newIndend;
    char ch;
    int r;

restart:
    if (tokenizer->state == NEW_LINE_STATE)
    {
        newIndend = read_new_line_indent(tokenizer); // read cur line intend
        r = tgetc(tokenizer, &ch);
        if (r == EOF)
        {
            tokenizer->state = EOF_STATE;
            tokenizer->newIndent = 0;
        }
        else if (ch == '\n') // empty line
        {
            goto restart;
        }
        else
        {
            tokenizer->newIndent = newIndend;
            tokenizer->state = INSIDE_LINE_STATE;
        }
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

    if (tokenizer->state == EOF_STATE)
        return make_empty_token();

    ch = '\0';
    while (tgetc(tokenizer, &ch) != EOF)
    {
        if (!is_pass_symbol(ch))
            break;
    }
    if (ch == '\0')
        return make_token(tokenizer, EOF_TOKEN);
    if (ch == '\n')
        return make_token(tokenizer, NEWLINE);
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
    if (is_digit(ch))
    {
        TToken numberToken = read_number(tokenizer);
        return numberToken;
    }
    if (is_string_start_symbol(ch))
    {
        TToken stringToken = read_string_token(tokenizer, ch);
        return stringToken;
    }
}
