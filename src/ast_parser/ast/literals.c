#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "ast_types.h"
#include "data_arena.h"
#include "system_tools.h"

#include "../lexer/tokenizer_api.h"
#include "../parser.h" //???
#include "ast.h"

bool is_bool_ident(TToken ident)
{
    assert(ident.type == IDENT);
    if (check_ident_string(ident, "False") || check_ident_string(ident, "True"))
        return true;
    return false;
}