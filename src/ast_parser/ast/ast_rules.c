#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ast_parser_api.h"
#include "ast_types.h"

#include "../parser.h"
#include "ast.h"

#define RULE_PREFIX()         \
    if (is_critical_error(p)) \
        return NULL;

#define DONE_RULE_POSTFIX()          \
    done:                            \
    flush_used_tokens(p->tokenizer); \
    return res;

static TNode *statements_rule(TAstParser *p);
static TNode *statement_rule(TAstParser *p);
static TNode *assign_rule(TAstParser *p);
static TNode *func_run_rule(TAstParser *p);
static TNode *expr_rule(TAstParser *p);
static TNode *term_rule(TAstParser *p);
static TNode *factor_rule(TAstParser *p);
static TNode *unsigned_factor_rule(TAstParser *p);
static TNode *pow_rule(TAstParser *p);
static TNode *primary_rule(TAstParser *p);
static TNode *number_rule(TAstParser *p);
static TNode *string_rule(TAstParser *p);
static TNode *ident_rule(TAstParser *p);

// if we can read rule => flush tokens

TNode *file_rule(TAstParser *p)
{
    if (is_critical_error(p))
        return NULL;

    rewind_tokenizer_pos(p->tokenizer);
    return statements_rule(p);
}

static TNode *statements_rule(TAstParser *p)
{
    if (is_critical_error(p))
        return NULL;

    TDataArena *arena = get_parser_arena(p);

    size_t length = 0;
    size_t capacity = BASE_STATETEMENTS_SIZE;
    TNode **statements = (TNode **)arena_malloc(arena, capacity * sizeof(TNode *));

    TNode *statement = NULL;
    while ((statement = statement_rule(p)))
    {
        if (length == capacity)
        {
            size_t newCapacity = 2 * capacity;
            TNode **newStatements = (TNode **)arena_realloc(arena, statements, capacity * sizeof(TNode *), newCapacity * sizeof(TNode *));
            if (newStatements == NULL)
            {
                set_memory_crit_error(p);
                break;
            }
            statements = newStatements;
            capacity = newCapacity;
        }
        statements[length++] = statement;
    }
    if (length == 0)
    {
        arena_free(arena, statements);
        return NULL;
    }
    TNode *statementsNode = init_statements_node(arena, statements, length);
    if (statementsNode == NULL)
    {
        set_memory_crit_error(p);
        return NULL;
    }

    flush_used_tokens(p->tokenizer);
    return NULL;
}

static TNode *statement_rule(TAstParser *p)
{
    if (is_critical_error(p))
        return NULL;

    int curPos = get_tokenizer_pos(p->tokenizer);

    TNode *statement;
    if ((statement = assign_rule(p)))
    {
        goto done;
    }
    if ((statement = func_run_rule(p)))
    {
        goto done;
    }
    set_tokenizer_pos(p->tokenizer, curPos);

done:
    flush_used_tokens(p->tokenizer);
    return statement;
}

static TNode *assign_rule(TAstParser *p)
{
    if (is_critical_error(p))
        return NULL;

    TDataArena *arena = get_parser_arena(p);

    TNode *res = NULL;
    TNode *ident, *expr;

    if ((ident = read_ident(p)) &&
        lookahead(p, ASSIGN) &&
        (expr = expr_rule(p)))
    {
        res = init_bin_op_node(arena, OP_ASSIGN, ident, expr);
        goto done;
    }

done:
    flush_used_tokens(p->tokenizer);
    return res;
}

static TNode *func_run_rule(TAstParser *p)
{
    if (is_critical_error(p))
        return NULL;
    TNode *res = NULL;
    TNode* func, args;

    DONE_RULE_POSTFIX()
}

static TNode *read_args_rule(TAstParser *p)
{
    if (is_critical_error(p))
        return NULL;
    return NULL;
}

static TNode *expr_rule(TAstParser *p)
{
    if (is_critical_error(p))
        return NULL;
    return NULL;
}

static TNode *term_rule(TAstParser *p)
{
    if (is_critical_error(p))
        return NULL;
    return NULL;
}

static TNode *factor_rule(TAstParser *p)
{
    if (is_critical_error(p))
        return NULL;
    return NULL;
}

static TNode *unsigned_factor_rule(TAstParser *p)
{
    if (is_critical_error(p))
        return NULL;
    return NULL;
}

static TNode *pow_rule(TAstParser *p)
{
    if (is_critical_error(p))
        return NULL;
    return NULL;
}

static TNode *primary_rule(TAstParser *p)
{
    if (is_critical_error(p))
        return NULL;
    return NULL;
}

static TNode *number_rule(TAstParser *p)
{
    if (is_critical_error(p))
        return NULL;
    return NULL;
}

static TNode *string_rule(TAstParser *p)
{
    if (is_critical_error(p))
        return NULL;
    return NULL;
}

static TNode *ident_rule(TAstParser *p)
{
    if (is_critical_error(p))
        return NULL;
    return NULL;
}
