#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "ast_parser_api.h"
#include "ast_types.h"
#include "data_arena.h"

#include "ast.h"
#include "../parser.h"
#include "../lexer/tokenizer_api.h"

bool lookahead(TAstParser *parser)
{
    // TTokenizer* tok = parser->tokenizer;
    return false;
}

TNode *get_node(TDataArena *arena, TToken curToken, NodeTypes type)
{
    TNode *node = (TNode *)arena_malloc(arena, sizeof(TNode));
    if (node == NULL)
        return NULL;
    node->type = type;
    return node;
}

void delete_node(TDataArena* arena, TNode *node)
{
    arena_free(arena, node);
}

TNode *init_statements_node(TDataArena *arena, TToken curToken)
{
    TNode *node = get_node(arena, curToken, STATEMENTS_TYPE);
    if (node == NULL)
        return NULL;

    TStatements *nodeValuePtr = &node->nodeValue.statements;
    nodeValuePtr->statementsArr = arena_malloc(arena, BASE_STATETEMENTS_SIZE * sizeof(TStatement));
    if (nodeValuePtr->statementsArr == NULL)
    {
        delete_node(arena, node);
        return NULL;
    }

    nodeValuePtr->capacity = BASE_STATETEMENTS_SIZE;
    nodeValuePtr->length = 0;
    return node;
}
