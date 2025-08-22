#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "ast_parser_api.h"
#include "ast_types.h"
#include "data_arena.h"

#include "../lexer/tokenizer_api.h"
#include "../parser.h"
#include "ast.h"

bool lookahead(TAstParser *p, TokenTypes checkType)
{
    TTokenizer *tok = p->tokenizer;

    while (1)
    {
        TToken token = token_soft_read(tok);
        if (token.type == EOF_TOKEN)
        {
            return false;
        }
        if (is_tokenizer_error(tok))
        {
            TTokenizerError error = get_tokenizer_error(tok);
            if (error.type == MEMORY_ERROR)
            {
                set_memory_crit_error(p);
                return false;
            }
            append_tokenizer_error(p, error);
            pass_tokenizer_error(tok);
            continue;
        }
        return token.type == checkType;
    }
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

void delete_node(TDataArena *arena, TNode *node)
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
