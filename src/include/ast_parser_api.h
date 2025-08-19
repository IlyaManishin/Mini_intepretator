#pragma once

#include <stdbool.h>
#include <stdio.h>

#include "parser_errors.h"
#include "system_tools.h"
#include "ast_types.h"

#define BASE_ERR_ARR_SIZE 10

typedef struct TRespErrors
{
    TFileData _fileData; // to destroy with errors

    const char* critErrorText;
    bool isCritError;

    TTokenizerError *tokErrors;
    size_t tokErrCount;
    size_t tokErrCapacity;

    TAstParserError *astParserErrors;
    size_t astParserErrCount;
    size_t astParserErrCapacity;
} TRespErrors;

typedef struct TAstParserResp
{
    TRespErrors* errors;
    TAst* ast;
} TAstParserResp;

TAstParserResp *run_ast_parser_from_file(FILE *file, char *fileName);
void delete_ast_tree(TAst *ast);
void delete_parser_resp_errors(TRespErrors *errors);