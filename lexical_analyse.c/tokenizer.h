#ifndef TOKENIZATION_H
#define TOKENIZATION_H 1

#include <stdlib.h>

typedef enum TokenTypes
{
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
    CLASS_KW,  // class
    IMPORT_KW, // import
    FROM_KW,   // from
    AS_KW,     // as

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
    
    char* start;
    char* end;
    size_t length; // ???
    
} TToken;

typedef enum TokenizerStates{
    BASE_STATE,
    NEW_LINE_STATE
} TokenizerStates;

typedef struct TTokenizer{
    char* buf;
    char* cur;
    char* curLine;
    char* end;

    TokenizerStates state;
    int lastIndent;
    int curIndent;
    
    TToken* tokensBuf;
} TTokenizer;

#endif