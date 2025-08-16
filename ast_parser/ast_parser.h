#pragma once

#include <stdbool.h>
#include <stdio.h>

#include "ast_parser_api.h"
#include "system_tools.h"
#include "data_arena.h"

#include "lexical_analyze/tokenizer_api.h"

#define MAX_AST_PARSER_DEEP 1024
#define SUCC 0
#define ERR 1

typedef struct TAstParser
{
    TAstParserResp *resp;

    TTokenizer *tokenizer;
    int deep;
} TAstParser;

TAstParser *ast_parser_from_file_data(TFileData fileData, TAstParserResp *resp);
void delete_ast_parser(TAstParser *parser);
void run_ast_parser(TAstParser *parser);

bool is_critical_error(TAstParserResp *resp);
void set_critical_resp_error(TAstParserResp *resp, const char *msg);
void set_memory_crit_error(TAstParserResp *resp);
