#include "ast_parser.h"
#include "ast_parser_api.h"

void set_critical_resp_error(TAstParserResp *resp, const char *msg)
{
    TRespErrors *errors = resp->errors;
    errors->isCritError = true;
    errors->critErrorText = msg;
}

void set_memory_crit_error(TAstParserResp *resp)
{
    set_critical_resp_error(resp, "Memory limit exceded");
}

bool is_critical_error(TAstParserResp *resp)
{
    return resp->errors->isCritError;
}

bool append_ast_error()
{
}
