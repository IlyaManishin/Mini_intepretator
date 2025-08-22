#include <stdlib.h>

#include "ast_parser_api.h"
#include "ast_types.h"
#include "data_arena.h"

#include "ast.h"
#include "../lexer/tokenizer_api.h"
#include "../parser.h"

TAst *init_ast()
{
    TDataArena *astArena = get_data_arena();
    if (astArena == NULL)
        return NULL;

    TAst *ast = arena_malloc(astArena, sizeof(TAst));
    if (ast == NULL)
    {
        delete_arena(astArena);
        return NULL;
    }
    ast->astArena = astArena;
    ast->first = NULL;
    return ast;
}

void read_ast(TAstParser *parser)
{
    parser->ast = init_ast();
    if (parser->ast == NULL)
    {
        set_memory_crit_error(parser);
        return;
    }
    parser->ast->first= file_rule(parser);

}

void delete_ast(TAst *ast)
{
    delete_arena(ast->astArena);
}
