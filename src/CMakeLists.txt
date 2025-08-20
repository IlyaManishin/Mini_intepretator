add_subdirectory(ast_parser)
add_subdirectory(system_tools)
add_subdirectory(utils)

add_executable(main program/main.c)
target_link_libraries(main PRIVATE ast_parser system_tools utils)

set_target_properties(main PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/src)
