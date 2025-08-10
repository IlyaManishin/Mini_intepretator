#pragma once

#include <stdbool.h>
#include <stdio.h>

#include "parser_errors.h"
#include "system_tools.h"
#include "ast_types.h"

#define CRIT_ERR_MAX_LENGTH 128

typedef struct TRespErrors
{
    TFileData _fileData; // to destroy with errors

    const char* critErrorText;
    bool isCritError;

    TTokenizerError *tokenizerErrors;
    int tokErrCount;

    TAstParserError *astParserErrors;
    int astParserErrCount;
} TRespErrors;

typedef struct TAstParserResp
{
    TRespErrors* errors;
    TAst* ast;
} TAstParserResp;

TAstParserResp *run_ast_parser_from_file(FILE *file, char *fileName);
void delete_ast_tree(TAst *ast);
void delete_parser_resp_errors(TRespErrors *errors);