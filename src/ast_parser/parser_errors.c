#include "ast_parser_api.h"
#include "parser.h"

TParserErrors *init_errors(TFileData fileData)
{
    TParserErrors *errors = (TParserErrors *)malloc(sizeof(TParserErrors));
    if (errors == NULL)
        return NULL;

    errors->tokErrors = (TTokenizerError *)malloc(BASE_ERR_ARR_SIZE * sizeof(TTokenizerError));
    errors->astParserErrors = (TAstParserError *)malloc(BASE_ERR_ARR_SIZE * sizeof(TAstParserError));
    if (errors->tokErrors == NULL || errors->astParserErrors == NULL)
    {
        if (errors->tokErrors != NULL)
            free(errors->tokErrors);
        if (errors->astParserErrors != NULL)
            free(errors->astParserErrors);
        free(errors);
        return NULL;
    }

    errors->tokErrCapacity = BASE_ERR_ARR_SIZE;
    errors->astParserErrCapacity = BASE_ERR_ARR_SIZE;
    errors->tokErrCount = 0;
    errors->astParserErrCount = 0;

    errors->_fileData = fileData;
    return errors;
}

void delete_parser_errors(TParserErrors *errors)
{
    delete_file_data(errors->_fileData);

    if (errors->astParserErrors != NULL)
        free(errors->astParserErrors);
    if (errors->tokErrors != NULL)
        free(errors->tokErrors);

    free(errors);
}

void set_crit_error(TAstParser *parser, const char *msg)
{
    TCritError *critErr = parser->critErr;
    critErr->isError = true;
    strncpy(critErr->msg, msg, CRIT_ERR_MSG_LENGTH);
}

void set_memory_crit_error(TAstParser *parser)
{
    TCritError *critErr = parser->critErr;
    critErr->isError = true;
    no_memory_crit_message(critErr->msg);
}

bool is_critical_error(TAstParser *parser)
{
    return parser->critErr->isError;
}

bool append_ast_error()
{
    return false;
}

void no_file_crit_message(char *dest, char *fileName)
{
    snprintf(dest, CRIT_ERR_MSG_LENGTH, "Can't open file: %s", fileName);
}

void no_memory_crit_message(char *dest)
{
    snprintf(dest, CRIT_ERR_MSG_LENGTH, "Memory limit exceded");
}
