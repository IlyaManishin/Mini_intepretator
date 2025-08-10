#pragma once

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
    PASS_OP,
    COMMANDS_SEQ_OP,
    SUM,
    SUB
    //....
} OperationTypes;

typedef struct TNode
{
    NodeTypes type;
    union
    {
        OperationTypes op;
        TLiteral literal;
        TStatement statement;
    } nodeValue;

    TNode *childs;
    int childsCount;

    int line;
    int column;
} TNode;

typedef struct TAst
{
    TNode *first;
} TAst;