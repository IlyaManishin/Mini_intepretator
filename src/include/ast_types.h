#pragma once

#include "data_arena.h"

#include "internal/ast_literals.h"
#include "internal/ast_operations.h"
#include "internal/ast_statements.h"

typedef enum NodeTypes
{
    LITERAL_TYPE,
    OP_TYPE,
    STATEMENTS_TYPE,
} NodeTypes;

typedef struct TNode
{
    NodeTypes type;
    union
    {
        TOperation op;
        TLiteral literal;
        TStatements statements;
        TStatement statement;
    } nodeValue;

    int lineno;
    int col;
} TNode;

typedef struct TAst
{
    TDataArena *astArena;
    TNode *first;
} TAst;