#pragma once

#include <stdbool.h>
#include <stdio.h>

#include <system_tools.h>
#include <parser_errors.h>

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



typedef struct TAstParserResp{

} TAstParserResp;



