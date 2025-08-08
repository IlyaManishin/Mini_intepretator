#pragma once

#include <stdbool.h>
#include <stdio.h>

#include "ast_parser_api.h"
#include "system_tools.h"

#include "lexical_analyze/tokenizer_api.h"

typedef struct TAstParser
{
    TAstParserResp *respDest;

    TTokenizer *tokenizer;
} TAstParser;

TAstParser *ast_parser_from_file_data(TFileData fileData, TAstParserResp *respDest);
void delete_ast_parser(TAstParser *parser);
void run_ast_parser(TAstParser *parser);