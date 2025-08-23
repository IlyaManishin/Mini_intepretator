set(LEXER_PATH lexer)
set(AST_PATH ast)

add_library(lexer STATIC
    ${LEXER_PATH}/token_buffer.c
    ${LEXER_PATH}/tokenizer.c
    ${LEXER_PATH}/tokenizer_api.c
)

add_library(ast STATIC
    ${AST_PATH}/ast_rules.c
    ${AST_PATH}/literals.c
    ${AST_PATH}/ast_api.c
    ${AST_PATH}/ast_tools.c
)


add_library(ast_parser STATIC
    parser.c
    ast_parser_api.c
    parser_errors.c)
    
target_link_libraries(ast_parser PRIVATE lexer ast)
