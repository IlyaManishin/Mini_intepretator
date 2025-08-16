#include <stdio.h>
#include <stdlib.h>

#include "ast_parser_api.h"
#include "system_tools.h"

#include "ast_parser.h"
#include "ast_rules.h"
#include "lexical_analyze/tokenizer_api.h"

void delete_ast_parser(TAstParser *parser)
{
    delete_tokenizer(parser->tokenizer);
    free(parser);
}

TAstParser *ast_parser_from_file_data(TFileData fileData, TAstParserResp *resp)
{
    TTokenizer *tokenizer = tokenizer_from_file_data(fileData);
    if (tokenizer == NULL)
        return NULL;

    TAstParser *parser = (TAstParser *)malloc(sizeof(TAstParser));
    if (parser == NULL)
        return NULL;
    
    parser->tokenizer = tokenizer;
    parser->resp = resp;
    parser->deep = 0;
    return parser;
}

void run_ast_parser(TAstParser *parser)
{
    TAst *ast = parser->resp->ast;
    ast->first = file_rule(parser);
}
