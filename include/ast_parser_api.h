#pragma once

#include <stdbool.h>
#include <stdio.h>

#include "parser_errors.h"
#include "system_tools.h"
#include "ast_types.h"

#define CRIT_ERR_MAX_LENGTH 128

typedef struct TTokenizerError
{
    char *textMsg;

    bool withPos;
    TErrorBufferPos pos;
} TTokenizerError;

typedef struct TAstParserError
{
    char *textMsg;
    TErrorBufferPos pos;
    //???
} TAstParserError;

typedef struct TRespErrors
{
    TFileData _fileData; // to destroy with errors

    char critErrorText[CRIT_ERR_MAX_LENGTH];
    bool isCritError;

    TTokenizerError *tokenizerErrors;
    int tokErrCount;

    TAstParserError *astParserErrors;
    int astParserErrCount;
} TRespErrors;

typedef struct TAstParserResp
{
    TRespErrors errors;
    
} TAstParserResp;

TAstParserResp *run_ast_parser_from_file(FILE *file, char *fileName);
void delete_ast_parser_resp(TAstParserResp *resp);