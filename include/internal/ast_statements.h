enum StatementsTypes
{
    EXPRESSION_STMT,
    FUNC_RUN_STMT // temp
};

typedef struct EXPR_statement
{

} EXPR_statement;

typedef struct FUNC_RUN_statement
{

} FUNC_RUN_statement;

typedef struct TStatement
{
    enum StatementsTypes type;
    union
    {
        EXPR_statement expr;
        FUNC_RUN_statement funcRun;
    } value;

} TStatement;

typedef struct TStatements
{
    TStatement *statements;
    size_t length, capacity;
} TStatements;