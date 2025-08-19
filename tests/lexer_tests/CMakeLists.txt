add_executable(test_lexer
    tests_run.c
    tokens_read_tests.c
)

target_link_libraries(test_lexer PRIVATE ast_parser system_tools utils)

#file(GLOB TEST_FILES "${CMAKE_CURRENT_LIST_DIR}/lexical_test_files/*.txt")

# foreach(file ${TEST_FILES})
#     configure_file(${file} ${CMAKE_CURRENT_BINARY_DIR}/lexical_test_files/ COPYONLY)
# endforeach()