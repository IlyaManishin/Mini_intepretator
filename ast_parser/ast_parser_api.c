#include <string.h>

#include "ast_parser_api.h"
#include "system_tools.h"

#include "ast.h"

static void set_critical_resp_error(TAstParserResp *resp, const char *msg)
{
    resp->isCritError = true;
    strncpy(resp->critErrorText, msg, CRIT_ERR_MAX_LENGTH);
}

void delete_ast_parser_resp(TAstParserResp *resp)
{
    delete_file_data(resp->_fileData);

    free(resp->tokenizerErrors);
    free(resp->astParserErrors);
    free(resp);
}

TAstParserResp *run_ast_parser_from_file(FILE *file, char *fileName)
{
    TAstParserResp *resp = (TAstParserResp *)malloc(sizeof(TAstParserResp));
    if (resp == NULL)
        return NULL;

    TFileData fileData = read_file_data(file);
    if (fileData.str == NULL)
    {
        set_critical_resp_error(resp, "Can't open script file");
        return resp;
    }
    resp->_fileData = fileData;

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