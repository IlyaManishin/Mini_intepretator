add_executable(lexer_tests
    program.c
    tokens_read_tests.c
    buffer_tests.c
)

target_link_libraries(lexer_tests PRIVATE ast_parser system_tools utils)
set_target_properties(lexer_tests PROPERTIES 
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/tests
)
file(GLOB TEST_FILES "${CMAKE_CURRENT_LIST_DIR}/lexer_test_files/*.txt")
file(COPY ${TEST_FILES} DESTINATION ${CMAKE_BINARY_DIR}/bin/tests/lexer_test_files)