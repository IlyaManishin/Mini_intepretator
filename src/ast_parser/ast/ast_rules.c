#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ast_parser_api.h"
#include "ast_types.h"

#include "../parser.h"
#include "ast.h"

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
    // TDataArena* arena = get_parser_arena(p);
    // TStatements *statements = init_statements_node(p, );
    TStatements *statements = NULL;
    if (statements == NULL)
    {
        set_memory_crit_error(p);
    }

    return NULL;
}

static TNode *statement_rule(TAstParser *p)
{
    if (is_critical_error(p))
        return NULL;
    return NULL;
}

static TNode *assign_rule(TAstParser *p)
{
    if (is_critical_error(p))
        return NULL;
    return NULL;
}

static TNode *func_run_rule(TAstParser *p)
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
