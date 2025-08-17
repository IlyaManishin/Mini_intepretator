#pragma once

#include "data_arena.h"

#include "internal/ast_literals.h"
#include "internal/ast_statements.h"

typedef enum NodeTypes
{
    LITERAL_TYPE,
    OP_TYPE,
    STATEMENTS_TYPE,
} NodeTypes;

typedef enum OperationTypes
{
    OP_PASS,        

    OP_SUM,          // +
    OP_SUB,          // -
    OP_MUL,          // *
    OP_DIV,          // /
    OP_POW,          // **

    OP_ASSIGN,       // =
} OperationTypes;

typedef struct TNode
{
    NodeTypes type;
    union
    {
        OperationTypes op;
        TLiteral literal;
        TStatements statements;
        TStatement statement;
    } nodeValue;

    int line;
    int column;
} TNode;

typedef struct TAst
{
    TDataArena *astArena;
    TNode *first;
} TAst;