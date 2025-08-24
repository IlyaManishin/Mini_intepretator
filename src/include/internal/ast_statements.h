#pragma once

#include "internal/ast_expressions.h"

#define BASE_STATETEMENTS_SIZE 8

typedef struct TNode TNode;

enum StatementsTypes
{
    FUNC_RUN_STMT // temp
};


typedef struct FUNC_RUN_statement
{

} FUNC_RUN_statement;

typedef struct TStatement
{
    enum StatementsTypes type;
    union
    {
        FUNC_RUN_statement funcRun;
    } value;

} TStatement;

typedef struct TStatements
{
    TNode **statementsArr;
    size_t length;
} TStatements;