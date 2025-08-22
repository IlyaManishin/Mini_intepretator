#pragma once

#include "data_arena.h"
#include "ast_parser_api.h"

#include "../parser.h"  
#include "../lexer/tokenizer_api.h"

TNode *file_rule(TAstParser *p);
TNode *init_statements_node(TDataArena *arena, TToken curToken);