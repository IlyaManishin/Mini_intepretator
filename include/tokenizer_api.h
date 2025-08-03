#pragma once

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

typedef enum TokenTypes
{
    ERROR, //on error

    IDENT,
    NUMBER,
    STRING,

    RETURN_KW,   // return
    IF_KW,       // if
    ELSE_KW,     // else
    ELIF_KW,     // elif
    FOR_KW,      // for
    BREAK_KW,    // break
    CONTINUE_KW, // continue
    TRUE_KW,     // True
    NULL_KW,     // Null
    FALSE_KW,    // False
    AND_KW,      // and
    OR_KW,       // or
    NOT_KW,      // not

    WHILE_KW,  // while
    IN_KW,     // in
    PASS_KW,   // pass
    FUNC_KW,   // func == def
    // CLASS_KW,  // class
    IMPORT_KW, // import
    FROM_KW,   // from
    // AS_KW,     // as

    PLUS,           // +
    MINUS,          // -
    MULTY,          // *
    DIVIS,          // /
    DOUBLESLASH,    // //
    PERCENT,        // %
    EQ,             // ==
    NEQ,            // !=
    LT,             // <
    GT,             // >
    LEQ,            // <=
    GEQ,            // >=
    ASSIGN,         // =
    PLUS_ASSIGN,    // +=
    MINUS_ASSIGN,   // -=
    STAR_ASSIGN,    // *=
    SLASH_ASSIGN,   // /=
    PERCENT_ASSIGN, // %=

    LPAREN,   // (
    RPAREN,   // )
    LBRACE,   // {
    RBRACE,   // }
    LBRACKET, // [
    RBRACKET, // ]
    COMMA,    // ,
    DOT,      // .
    COLON,    // :

    INDENT,
    DEDENT,
    NEWLINE,
    EOF_TOKEN
} TokenTypes;

typedef struct TToken
{
    TokenTypes type;

    char *start;
    char *end; // for ident only

} TToken;

typedef enum TokenizerStates
{
    NEW_LINE_STATE,
    INSIDE_LINE_STATE,
    EOF_STATE,
} TokenizerStates;

typedef enum TokenizerErrors
{
    NONE,
    TOK_EOF,
    TOK_INVALID_TOKEN
} TokenizerErrors;

typedef struct TTokenizerError
{
    char *textMsg;

    bool withPos;
    char *errLine;
    int errLineIndex;
    int errColumn;
} TTokenizerError;

typedef struct TTokBuffer
{
    TToken *data;
    int length;
    int capacity;
} TTokBuffer;

typedef struct TTokenizer
{
    char *buf;
    char* start;
    char *cur;
    char *curLine; // start of code or after \n 
    int lineIndex;
    char *end;

    TokenizerStates state;
    int curIndent;
    int newIndent;

    TTokBuffer* tokensBuf;

    bool isError;
    TokenizerErrors errorType;
    TTokenizerError errMesg;
} TTokenizer;

TTokenizer *tokenizer_from_file(FILE *file);
void delete_tokenizer(TTokenizer *tokenizer);

TToken get_token(TTokenizer *tokenizer);
void back_token(TTokenizer *tokenizer, TToken token);

bool is_tokenizer_error(TTokenizer *tokenizer);
TTokenizerError make_pos_error(char *textMsg, char *errLine, int lineIndex, char *cur);
TTokenizerError make_base_error(char *textMsg);
TTokenizerError get_tokenizer_error(TTokenizer *tokenizer);
