#include <assert.h>
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

static TToken read_token(TAstParser *p)
{
    return token_soft_read(p->tokenizer);
}

static TNode *get_node(TDataArena *arena, NodeTypes type)
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

static char *copy_token_str(TDataArena *arena, TToken token, size_t *lengthDest)
{
    size_t length = token_strlen(token);
    assert(length != 0);

    char *res = (char *)arena_malloc(arena, length + 1);
    if (res == NULL)
        return NULL;

    memcpy(res, token.start, length);
    res[length] = '\0'; // need?
    if (lengthDest != NULL)
        *lengthDest = length;
    return res;
}

TNode *read_ident(TAstParser *p)
{
    int pos = get_tokenizer_pos(p->tokenizer);
    TToken token = read_token(p);
    if (token.type != IDENT || is_bool_ident(token))
    {
        set_tokenizer_pos(p->tokenizer, pos);
        return NULL;
    }

    TDataArena *arena = get_parser_arena(p);
    TNode *node = get_node(arena, LITERAL_TYPE);
    if (node == NULL)
    {
        goto memory_error;
    }
    size_t identLength;
    char *identName = copy_token_str(arena, token, &identLength);
    if (identName == NULL)
        goto memory_error;

    TString *asString = &node->nodeValue.literal.string;
    asString->data = identName;
    asString->length = identLength;
    return node;

memory_error:
    set_memory_crit_error(p);
    set_tokenizer_pos(p->tokenizer, pos);
    return NULL;
}

TNode *init_statements_node(TDataArena *arena, TNode **statements, int length)
{
    TNode *node = get_node(arena, STATEMENTS_TYPE);
    if (node == NULL)
        return NULL;

    TStatements *nodeValuePtr = &node->nodeValue.statements;
    nodeValuePtr->statementsArr = statements;
    nodeValuePtr->length = length;
    return node;
}

TNode *init_bin_op_node(TDataArena *arena, OperationTypes opType, TNode *left, TNode *right)
{
    TNode *node = get_node(arena, OP_TYPE);
    if (node == NULL)
        return NULL;

    TOperation *asOperation = &node->nodeValue.op;
    asOperation->type = opType;
    asOperation->left = left;
    asOperation->right = right;

    return node;
}
