#include <stdlib.h>
#include <string.h>

#include "ast_parser.h"
#include "ast_parser_api.h"
#include "ast_rule_tools.h"
#include "ast_types.h"
#include "data_arena.h"

#include "lexical_analyze/tokenizer_api.h"

TNode *get_node(TDataArena *arena, TToken curToken, NodeTypes type)
{
    TNode* node = (TNode *)arena_malloc(arena, sizeof(TNode));
    if (node == NULL) return NULL;
    node->type = type;
    return node;
}

TNode *init_statements_node(TDataArena *arena, TToken curToken)
{
    TNode *node = get_node(arena, curToken, STATEMENTS_TYPE);
    if (node == NULL)
        return NULL;
    
    TStatements* nodeValue = &node->nodeValue.statements;
    nodeValue->statementsArr = (TStatement *)malloc(BASE_STATETEMENTS_SIZE * sizeof(TStatement));
    nodeValue->capacity = BASE_STATETEMENTS_SIZE;
    nodeValue->length = 0;
    return node;
}

