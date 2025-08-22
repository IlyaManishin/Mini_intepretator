#include "ast_parser_api.h"
#include "syntax_errors.h"
#include "system_tools.h"

#include "ast/ast_api.h"
#include "parser.h"

static TParserResp *init_empty_parser_resp()
{
    TParserResp *resp = (TParserResp *)malloc(sizeof(TParserResp));
    if (resp == NULL)
        return NULL;
    resp->critError.isError = false;

    resp->ast = NULL;
    resp->errors = NULL;
    return resp;
}

void delete_parser_resp(TParserResp *resp)
{
    if (resp->ast != NULL)
        delete_ast(resp->ast);
    if (resp->errors != NULL)
        delete_parser_errors(resp->errors);
    free(resp);
}

static void set_resp_crit_error(TParserResp *resp, const char *msg)
{
    resp->critError.isError = true;
    strncpy(resp->critError.msg, msg, CRIT_ERR_MSG_LENGTH);
}

static TParserResp *get_error_resp(const char *msg)
{
    TParserResp *resp = init_empty_parser_resp();
    if (resp == NULL)
        return NULL;

    resp->errors = NULL;
    resp->ast = NULL;
    set_resp_crit_error(resp, msg);
    return resp;
}

TParserResp *run_ast_parser_from_file(FILE *file, char *fileName)
{
    TFileData fileData = read_file_data(file);
    if (fileData.str == NULL)
    {
        char msg[CRIT_ERR_MSG_LENGTH + 1];
        no_file_crit_message(msg, fileName);

        TParserResp *errorResp = get_error_resp(msg);
        return errorResp;
    }

    TParserResp *resp = init_empty_parser_resp();
    if (resp == NULL)
    {
        delete_file_data(fileData);
        return NULL;
    }

    resp->errors = init_errors(fileData);
    if (resp->errors == NULL)
    {
        resp->critError.isError = true;
        delete_file_data(fileData);
        no_memory_crit_message(resp->critError.msg);
        return resp;
    }

    TAstParser *parser = ast_parser_from_file_data(fileData, resp->errors, &resp->critError);
    if (parser == NULL)
    {
        resp->critError.isError = true;
        no_memory_crit_message(resp->critError.msg);
        return resp;
    }

    if (!is_critical_error(parser))
        run_ast_parser(parser);

    resp->ast = parser->ast;

    delete_ast_parser(parser);
    return resp;
}