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

TAstParser *ast_parser_from_file_data(TFileData fileData);
void delete_ast_parser(TAstParser *parser);
void run_ast_parser(TAstParser *parser);

TDataArena *get_parser_arena(TAstParser *parser);

TParserErrors *init_errors(TFileData fileData);
void delete_parser_errors(TParserErrors *errors);

TCritError *init_crit_error();
void set_crit_error(TAstParser* parser, const char *msg);
void set_memory_crit_error(TAstParser* parser);
bool is_critical_error(TAstParser* parser);
void delete_crit_error(TCritError *critErr);

void no_file_crit_message(char *dest, char *fileName);
void no_memory_crit_message(char *dest);

