cmake_minimum_required(VERSION 3.20)
project(Interpretator C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# add_compile_options(-Wall -Wextra -Wpedantic -g)
add_compile_options(-g)


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
include_directories(${CMAKE_SOURCE_DIR}/src/include)

add_subdirectory(src)   
add_subdirectory(tests)   



