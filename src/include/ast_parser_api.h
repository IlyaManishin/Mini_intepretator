#pragma once

#include <stdbool.h>
#include <stdio.h>

#include "ast_types.h"
#include "syntax_errors.h"
#include "system_tools.h"

#define BASE_ERR_ARR_SIZE 10

typedef struct TParserErrors
{
    TFileData _fileData; // to destroy with errors

    TTokenizerError *tokErrors;
    size_t tokErrCount;
    size_t tokErrCapacity;

    TAstParserError *astParserErrors;
    size_t astParserErrCount;
    size_t astParserErrCapacity;
} TParserErrors;

typedef struct TParserResp
{
    TCritError *critError;
    TParserErrors *errors;
    TAst *ast;
} TParserResp;

TParserResp *run_ast_parser_from_file(FILE *file, char *fileName);
void delete_parser_resp(TParserResp *resp);