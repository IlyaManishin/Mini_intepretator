#include <string.h>

#include "ast_parser_api.h"
#include "system_tools.h"

#include "ast_parser.h"

static TAst *init_ast()
{
    TAst *ast = (TAst *)malloc(sizeof(TAst));
    if (ast == NULL)
        return NULL;
    ast->first = NULL;
    return ast;
}

static TRespErrors *init_errors()
{
    TRespErrors *errors = (TRespErrors *)malloc(sizeof(TRespErrors));
    if (errors == NULL)
        return NULL;

    errors->tokErrors = (TTokenizerError *)malloc(BASE_ERR_ARR_SIZE * sizeof(TTokenizerError));
    errors->astParserErrors = (TAstParserError *)malloc(BASE_ERR_ARR_SIZE * sizeof(TAstParserError));
    if (errors->tokErrors == NULL || errors->astParserErrors == NULL)
        return NULL;

    errors->tokErrCapacity = BASE_ERR_ARR_SIZE;
    errors->astParserErrCapacity = BASE_ERR_ARR_SIZE;
    errors->tokErrCount = 0;
    errors->astParserErrCount = 0;

    return errors;
}

static TAstParserResp *init_ast_parser_resp()
{
    TAstParserResp *resp = (TAstParserResp *)malloc(sizeof(TAstParserResp));
    if (resp == NULL)
        return NULL;

    resp->ast = init_ast();
    if (resp->ast == NULL)
        return NULL;

    resp->errors = init_errors();
    if (resp->errors == NULL)
        return NULL;

    resp->errors->tokErrCount = 0;
    resp->errors->astParserErrCount = 0;
    return resp;
}

static void set_critical_resp_error(TAstParserResp *resp, const char *msg)
{
    TRespErrors *errors = resp->errors;
    errors->isCritError = true;
    errors->critErrorText = msg;
}

void delete_parser_resp_errors(TRespErrors *errors)
{
    delete_file_data(errors->_fileData);
    free(errors->tokErrors);
    free(errors->astParserErrors);
    free(errors);
}

void delete_ast_tree(TAst *ast)
{
    //???
}

TAstParserResp *run_ast_parser_from_file(FILE *file, char *fileName)
{
    TAstParserResp *resp = init_ast_parser_resp();
    if (resp == NULL)
        return NULL;

    TFileData fileData = read_file_data(file);
    if (fileData.str == NULL)
    {
        set_critical_resp_error(resp, "Can't open script file");
        return resp;
    }
    resp->errors->_fileData = fileData;

    TAstParser *parser = ast_parser_from_file_data(fileData, resp);
    if (parser == NULL)
    {
        set_critical_resp_error(resp, "Memory limit exceded");
        return resp;
    }
    run_ast_parser(parser);
    delete_ast_parser(parser);

    return resp;
}