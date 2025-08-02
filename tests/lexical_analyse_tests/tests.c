#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include <tokenizer_api.h>

#define STRNCMP_MAX 1024
#define SUCC 1
#define ERR 0

static int testIndex = 1;

char *get_ident_name(TToken token)
{
    size_t length = token.end - token.start;
    char *name = (char *)malloc((length + 1) * sizeof(char));

    for (int i = 0; i < length; i++)
    {
        name[i] = *(token.start + i);
    }
    name[length] = '\0';
    return name;
}

int check_token(TTokenizer *tok, TokenTypes check, const char *checkName)
{
    TToken token = read_token(tok);
    if (check == IDENT)
    {
        if (token.type != IDENT)
            return 0;
        char *name = get_ident_name(token);
        int res = strcmp(name, checkName);
        if (res != 0)
        {
            printf("Incorrect ident name: %s. Expected: %s\n", name, checkName);
        }
        free(name);
        return res == 0 ? SUCC : ERR;
    }
    else
    {
        return token.type == check ? SUCC : ERR;
    }
}

int test1()
{
    int result = SUCC;
    FILE *file = fopen("test_files/test1.txt", "r");
    TTokenizer *tokenizer = tokenizer_from_file(file);

    struct CheckData
    {
        TokenTypes check;
        const char *str
    } data[] = {

    };
    int n = sizeof(data) / sizeof(data[0]);
    for (int i = 0; i < n; i++)
    {
        int r = check_token(tokenizer, data[i].check, data[i].str);
        if (r == ERR)
        {
            result = ERR;
            break;
        }
    }
    fclose(file);
    return result;
}

int test2()
{
    return SUCC;
}

int test3()
{
    return SUCC;
}

int run_test(int (*test)())
{
    int result = test();
    if (result)
    {
        printf("%d) Success: test%d\n\n", testIndex, testIndex);
    }
    else
    {
        printf("%d) ERROR: test%d\n\n", testIndex, testIndex);
    }
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