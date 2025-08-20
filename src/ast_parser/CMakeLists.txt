set(LEXER_PATH lexer)

add_library(lexer STATIC
    ${LEXER_PATH}/token_buffer.c
    ${LEXER_PATH}/tokenizer.c
    ${LEXER_PATH}/tokenizer_api.c
)

add_library(ast_parser STATIC
    ast_error.c
    ast_parser_api.c
    ast_rule_tools.c
    ast_rules.c)
    
target_link_libraries(ast_parser PRIVATE lexer)
