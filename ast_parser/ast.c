#include <stdio.h>
#include <stdlib.h>

#include "ast_parser_api.h"
#include "system_tools.h"

#include "ast.h"
#include "lexical_analyze/tokenizer_api.h"

void delete_ast_parser(TAstParser *parser)
{
    delete_tokenizer(parser->tokenizer);
    free(parser);
}

TAstParser *ast_parser_from_file_data(TFileData fileData, TAstParserResp *respDest)
{
    TTokenizer *tokenizer = tokenizer_from_file_data(fileData);
    if (tokenizer == NULL)
        return NULL;

    TAstParser *parser = (TAstParser *)malloc(sizeof(TAstParser));
    parser->tokenizer = tokenizer;
    parser->respDest = respDest;

    return parser;
}

void run_ast_parser(TAstParser *parser)
{
    //???
}
