#pragma once

#include "ast_parser_api.h"
#include "data_arena.h"

#include "../lexer/tokenizer_api.h"
#include "../parser.h"

TNode *file_rule(TAstParser *p);

bool lookahead(TAstParser *p, TokenTypes checkType);

TNode *init_statements_node(TDataArena *arena, TNode **statements, int length);

bool check_ident_string(TToken ident, char *str);
bool is_bool_ident(TToken ident);