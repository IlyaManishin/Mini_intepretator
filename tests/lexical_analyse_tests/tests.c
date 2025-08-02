#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include <tokenizer_api.h>

#define PATH_MAX 128
#define STRNCMP_MAX 1024
#define SUCC 1
#define ERR 0

typedef struct TCheckData
{
    TokenTypes check;
    const char *str;
} TCheckData;

static int testIndex = 1;

char *get_token_string(TToken token)
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

int check_token(TTokenizer *tok, int index, TokenTypes check, const char *checkName)
{
    TToken token = read_token(tok);
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
}

int base_test(const TCheckData data[], int n)
{
    int result = SUCC;

    char filePath[PATH_MAX];
    snprintf(filePath, PATH_MAX, "test_files/test%d.txt", testIndex);
    FILE *file = fopen(filePath, "r");

    TTokenizer *tokenizer = tokenizer_from_file(file);
    for (int i = 0; i < n; i++)
    {
        int r = check_token(tokenizer, i, data[i].check, data[i].str);
        if (r == ERR)
        {
            result = ERR;
            break;
        }
    }
    if (check_token(tokenizer, 0, EOF_TOKEN, NULL) == ERR)
        result = ERR;
    fclose(file);
    return result;
}

int test1()
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
    return base_test(data, n);
}

int test2()
{
    TCheckData data[] = {
        {IMPORT_KW, NULL},
        {IDENT, "foo"},
        {NEWLINE, NULL},
        {FUNC_KW, NULL},
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
    return base_test(data, n);
}

int test3()
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
    return base_test(data, n);
}

int run_test(int (*test)())
{
    int result = test();
    if (result)
    {
        printf("%d) Test%d : Success\n", testIndex, testIndex);
    }
    else
    {
        printf("%d) Test%d : ERROR\n", testIndex, testIndex);
    }
    printf("_______________________________\n");
    testIndex++;
    return result;
}
int main()
{
    int passedCount = 0;
    passedCount += run_test(test1);
    passedCount += run_test(test2);
    passedCount += run_test(test3);
    printf("PASSED: %d/%d\n", passedCount, testIndex - 1);
}