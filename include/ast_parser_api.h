#pragma once

#include <stdbool.h>
#include <stdio.h>

#include "parser_errors.h"
#include "system_tools.h"

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

typedef struct TAstParserResp
{
    char critErrorText[CRIT_ERR_MAX_LENGTH];
    bool isCritError;
    
    TTokenizerError *tokenizerErrors;
    int tokErrCount;

    TAstParserError* astParserErrors;
    int astParserErrCount;

    TFileData _fileData; // to destroy with response
} TAstParserResp;
