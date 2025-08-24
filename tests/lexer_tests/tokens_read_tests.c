#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

// #include "ast_parser_api.h"
#include "syntax_errors.h"
#include "system_tools.h"

#include "../../src/ast_parser/lexer/tokenizer_api.h"

#define PATH_MAX 128
#define SUCC 1
#define ERR 0

typedef struct TCheckData
{
    TokenTypes check;
    const char *str;
} TCheckData;

static int testIndex = 1;
static int withValidErrorsPrint = true;

static char *get_token_string(TToken token)
{
    size_t length = token.end - token.start;
    char *tokenStr = (char *)malloc((length + 1) * sizeof(char));

    for (int i = 0; i < length; i++)
    {
        tokenStr[i] = *(token.start + i);
    }
    tokenStr[length] = '\0';
    return tokenStr;
}

static int check_token(TTokenizer *tok,
                int index,
                TokenTypes check,
                const char *checkName,
                bool isSilent,
                TToken (*read_token_function)(TTokenizer *tok))
{
    TToken token = read_token_function(tok);
    if (is_tokenizer_error(tok))
    {
        if (check != ERROR_TOKEN || withValidErrorsPrint)
        {
            TTokenizerError error = get_tokenizer_error(tok);
            if (!isSilent)
            {
                if (error.withPos)
                    print_error_with_pos(error.textMsg, error.pos);
                else
                    print_error_msg(error.textMsg);
            }
        }
        pass_tokenizer_error(tok);
    }
    if (token.type != check)
    {
        printf("(%d) Incorrect type: %d. Expected: %d\n", index, token.type, check);
        return ERR;
    }
    if (check == IDENT || check == NUMBER)
    {
        char *tokenStr = get_token_string(token);
        int res = strcmp(tokenStr, checkName);
        if (res != 0)
        {
            if (check == IDENT)
                printf("(%d) Incorrect ident name: %s. Expected: %s\n", index, tokenStr, checkName);
            else if (check == NUMBER)
            {
                printf("(%d) Incorrect number: %s. Expected: %s\n", index, tokenStr, checkName);
            }
        }
        free(tokenStr);
        return res == 0 ? SUCC : ERR;
    }
    return SUCC;
}

static int base_test(const TCheckData data[], int n, bool isSilent, TToken (*read_token_function)(TTokenizer *tok))
{
    int result = SUCC;

    char filePath[PATH_MAX];
    snprintf(filePath, PATH_MAX, "lexer_test_files/token_test%d.txt", testIndex);
    FILE *file = fopen(filePath, "r");
    TFileData fileData = read_file_data(file);
    assert(fileData.str != NULL);

    TTokenizer *tokenizer = tokenizer_from_file_data(fileData);
    for (int i = 0; i < n; i++)
    {
        int r = check_token(tokenizer, i, data[i].check, data[i].str, isSilent, read_token_function);
        if (r == ERR)
        {
            result = ERR;
            break;
        }
    }
    if (check_token(tokenizer, 0, EOF_TOKEN, NULL, isSilent, read_token_function) == ERR)
        result = ERR;

    delete_tokenizer(tokenizer);
    delete_file_data(fileData);
    fclose(file);
    return result;
}

static int test1()
{
    TCheckData data[] = {
        {FOR_KW, NULL},
        {IDENT, "i"},
        {IN_KW, NULL},
        {IDENT, "range"},
        {LPAREN, NULL},
        {NUMBER, "1"},
        {COMMA, NULL},
        {NUMBER, "3"},
        {RPAREN, NULL},
        {COLON, NULL},
        {NEWLINE, NULL},
        {INDENT, NULL},
        {IDENT, "print"},
        {LPAREN, NULL},
        {NUMBER, "5"},
        {RPAREN, NULL},
        {DEDENT, NULL}};
    int n = sizeof(data) / sizeof(data[0]);
    return base_test(data, n, false, token_soft_read);
}

static int test2()
{
    TCheckData data[] = {
        {IMPORT_KW, NULL},
        {IDENT, "foo"},
        {NEWLINE, NULL},
        {DEF_KW, NULL},
        {IDENT, "f"},
        {LPAREN, NULL},
        {RPAREN, NULL},
        {COLON, NULL},
        {NEWLINE, NULL},
        {INDENT, NULL},
        {IDENT, "print"},
        {LPAREN, NULL},
        {NUMBER, "13.4"},
        {RPAREN, NULL},
        {NEWLINE, NULL},
        {DEDENT, NULL}};
    int n = sizeof(data) / sizeof(data[0]);
    return base_test(data, n, false, token_soft_read);
}

static int test3()
{
    TCheckData data[] = {
        {IDENT, "a"},
        {PLUS_ASSIGN, NULL},
        {IDENT, "b"},
        {NEWLINE, NULL},

        {IDENT, "a"},
        {ASSIGN, NULL},
        {NUMBER, "4"},
        {PLUS, NULL},
        {NUMBER, "7"},
        {NEWLINE, NULL},

        {IDENT, "c"},
        {ASSIGN, NULL},
        {STRING, "boo"}};
    int n = sizeof(data) / sizeof(data[0]);
    return base_test(data, n, false, token_soft_read);
}

static int test4()
{
    // strings test
    TCheckData data[] = {
        {IDENT, "a"},
        {ASSIGN, NULL},
        {STRING, "\"\\\\\\\"\\\\"}, // строка: "\\\"\\"
        {NEWLINE, NULL},

        {IDENT, "b"},
        {ASSIGN, NULL},
        {STRING, "foo\"boo\"foo"},
    };
    int n = sizeof(data) / sizeof(data[0]);
    return base_test(data, n, false, token_soft_read);
}

static int test5()
{
    TCheckData data[] = {
        {IDENT, "a"},
        {ASSIGN, NULL},
        {ERROR_TOKEN, NULL},
        {NEWLINE, NULL},

        {IDENT, "b"},
        {ASSIGN, NULL},
        {ERROR_TOKEN, NULL},
    };
    int n = sizeof(data) / sizeof(data[0]);
    return base_test(data, n, false, token_soft_read);
}

static int test6()
{
    TCheckData data[] = {
        {IDENT, "a"},
        {ASSIGN, NULL},
        {IDENT, "b"},
        {LEQ, NULL},
        {ASSIGN, NULL},
        {IDENT, "c"},
        {NEWLINE, NULL},

        {IDENT, "b"},
        {ERROR_TOKEN, NULL},
        {IDENT, "c"},
        {NEWLINE, NULL},

        {IDENT, "b"},
        {ERROR_TOKEN, NULL},
        {IDENT, "c"}};
    int n = sizeof(data) / sizeof(data[0]);
    return base_test(data, n, false, token_soft_read);
}

static int run_test(int (*test)())
{
    int result = test();
    if (result)
    {
        printf("%d) TEST%d : Success\n", testIndex, testIndex);
    }
    else
    {
        printf("%d) TEST%d : ERROR\n", testIndex, testIndex);
    }
    printf("_______________________________\n");
    testIndex++;
    return result;
}

void run_tokens_read_tests()
{
    int passedCount = 0;
    passedCount += run_test(test1);
    passedCount += run_test(test2);
    passedCount += run_test(test3);
    passedCount += run_test(test4);
    passedCount += run_test(test5);
    passedCount += run_test(test6);


    printf("=== Tokens read_tests, %d/%d PASSED ===\n", passedCount, testIndex - 1);
}