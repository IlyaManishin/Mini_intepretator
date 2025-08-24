#pragma once

typedef struct TNode TNode;

typedef enum OperationTypes
{
    OP_PASS,

    OP_SUM, // +
    OP_SUB, // -
    OP_MUL, // *
    OP_DIV, // /
    OP_POW, // **

    OP_ASSIGN, // =
} OperationTypes;

typedef struct TOperation
{
    OperationTypes type;
    TNode *left;
    TNode *right;
} TOperation;