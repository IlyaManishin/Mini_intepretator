#include "assert.h"

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

void no_file_crit_message(char *dest, char *fileName)
{
    snprintf(dest, CRIT_ERR_MSG_LENGTH, "Can't open file: %s", fileName);
}

void no_memory_crit_message(char *dest)
{
    snprintf(dest, CRIT_ERR_MSG_LENGTH, "Memory limit exceded");
}

// static void *increase_error_capacity(TDataArena *arena, size_t capacityPtr, )
// {
// }

// bool append_tokenizer_error(TAstParser *parser, TTokenizerError tokError)
// {
//     assert(parser->errors != NULL);

//     TParserErrors *errors = parser->errors;

//     if (errors->tokErrCount == errors->tokErrCapacity)
//     {
//         TTokenizerError *newTokErrors =
//     }
// }

// bool append_ast_error(TAstParser *parser, TAstParserError astError)
// {
//     assert(parser->errors != NULL);
//     return false;
// }

static void *increase_error_capacity(TDataArena *arena, void *arr, size_t *capacity, size_t elemSize)
{
    size_t newCapacity = *capacity * 2;
    void *newArr = arena_realloc(arena, arr, (*capacity) * elemSize, newCapacity * elemSize);
    if (newArr == NULL)
        return NULL;

    *capacity = newCapacity;
    return newArr;
}

bool append_tokenizer_error(TAstParser *parser, TTokenizerError tokError)
{
    assert(parser->errors != NULL);

    TParserErrors *errors = parser->errors;
    TDataArena *arena = get_parser_arena(parser);

    if (errors->tokErrCount == errors->tokErrCapacity)
    {
        TTokenizerError *newArr = increase_error_capacity(arena,
                                                          errors->tokErrors,
                                                          &errors->tokErrCapacity,
                                                          sizeof(TTokenizerError));
        if (newArr == NULL)
            return false;
        errors->tokErrors = newArr;
    }

    errors->tokErrors[errors->tokErrCount++] = tokError;
    return true;
}

bool append_ast_error(TAstParser *parser, TAstParserError astError)
{
    assert(parser->errors != NULL);

    TParserErrors *errors = parser->errors;
    TDataArena *arena = get_parser_arena(parser);

    if (errors->astParserErrCount == errors->astParserErrCapacity)
    {
        TAstParserError *newArr = increase_error_capacity(arena,
                                                          errors->astParserErrors,
                                                          &errors->astParserErrCapacity,
                                                          sizeof(TAstParserError));
        if (newArr == NULL)
            return false;
        errors->astParserErrors = newArr;
    }

    errors->astParserErrors[errors->astParserErrCount++] = astError;
    return true;
}
