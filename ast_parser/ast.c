#include <stdio.h>
#include <stdlib.h>

#include <ast_parser_api.h>
#include <system_tools.h>

#include "lexical_analyze/tokenizer_api.h"
#include "ast.h"

void delete_ast_parser(TAstParser *parser)
{
    delete_file_data(parser->fileData);
    delete_tokenizer(parser->tokenizer);
    free(parser);
}

TAstParser *ast_parser_from_file(FILE *scriptFile, bool passTokenizerWarnings)
{
    TFileData fileData = read_file_data(scriptFile);
    if (fileData.data == NULL || fileData.dataSize == 0)
        return NULL;
    TTokenizer *tokenizer = tokenizer_from_str(fileData.data, fileData.dataSize);
    if (tokenizer == NULL)
        return NULL;

    TAstParser *parser = (TAstParser *)malloc(sizeof(TAstParser));
    parser->fileData = fileData;
    parser->tokenizer = tokenizer;
    parser->passTokenizerWarnings = passTokenizerWarnings;

    return parser;
}

