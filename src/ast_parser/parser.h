#pragma once

#include <stdbool.h>
#include <stdio.h>

#include "ast_parser_api.h"
#include "syntax_errors.h"
#include "system_tools.h"
#include "data_arena.h"

#include "lexer/tokenizer_api.h"

#define MAX_AST_PARSER_DEEP 1024
#define SUCC 0
#define ERR 1

typedef struct TAstParser
{
    TTokenizer *tokenizer;
    TAst* ast;
    
    TCritError* critErr;
    TParserErrors* errors;
    
    int deep;
} TAstParser;

TAstParser *ast_parser_from_file_data(TFileData fileData, TParserErrors *errors, TCritError *critErr);
void delete_ast_parser(TAstParser *parser);
void run_ast_parser(TAstParser *parser);

TDataArena *get_parser_arena(TAstParser *parser);

TParserErrors *init_errors(TFileData fileData);
bool append_tokenizer_error(TAstParser *parser, TTokenizerError tokError);
bool append_ast_error(TAstParser *parser, TAstParserError astError);
void delete_parser_errors(TParserErrors *errors);

void set_crit_error(TAstParser* parser, const char *msg);
void set_memory_crit_error(TAstParser* parser);
bool is_critical_error(TAstParser* parser);

void no_file_crit_message(char *dest, char *fileName);
void no_memory_crit_message(char *dest);

