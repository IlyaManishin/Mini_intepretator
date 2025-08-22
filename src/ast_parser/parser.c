#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "ast_parser_api.h"
#include "system_tools.h"

#include "parser.h"
#include "ast/ast_api.h"
#include "lexer/tokenizer_api.h"

void delete_ast_parser(TAstParser *parser)
{
    if (parser->tokenizer != NULL)
        delete_tokenizer(parser->tokenizer);
    free(parser);
}

static TAstParser *init_parser()
{
    TAstParser *parser = (TAstParser *)malloc(sizeof(TAstParser));
    if (parser == NULL)
        return NULL;

    parser->ast = NULL;
    parser->errors = NULL;
    parser->tokenizer = NULL;

    return parser;
}

TAstParser *ast_parser_from_file_data(TFileData fileData)
{
    TAstParser *parser = init_parser();
    if (parser == NULL)
        return NULL;

    parser->errors = init_errors(fileData);
    if (parser->errors == NULL)
    {
        delete_ast_parser(parser);
        return NULL;
    }

    parser->tokenizer = tokenizer_from_file_data(fileData);
    if (parser->tokenizer == NULL)
    {
        set_memory_crit_error(parser);
        return parser;
    }

    parser->deep = 0;
    return parser;
}

void run_ast_parser(TAstParser *parser)
{
    assert(is_critical_error(parser));


}

TDataArena *get_parser_arena(TAstParser *parser)
{
    assert(parser->ast != NULL);
    return parser->ast->astArena;
}
