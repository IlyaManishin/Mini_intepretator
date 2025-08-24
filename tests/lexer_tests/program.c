#include <stdio.h>

#include "tests.h"

int main()
{
    printf("=== RUN LEXER TEST ===\n\n");
    
    run_tokens_read_tests();
    run_buffer_tests();

    printf("=== EXIT LEXER TEST ===");

}