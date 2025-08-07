#pragma once

#include <stdbool.h>
#include <stdio.h>

#include <system_tools.h>

#include "lexical_analyze/tokenizer_api.h"

typedef struct TAstParser
{
    TFileData fileData;
    
    TTokenizer *tokenizer;
    bool passTokenizerWarnings;
} TAstParser;
