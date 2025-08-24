#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "data_arena.h"

#include "../../src/ast_parser/lexer/token_buffer.h"
#include "../../src/ast_parser/lexer/tokenizer_api.h"

/* REQUIRE: если условие ложно — прерывает текущий тест с кодом 1,*/
#define REQUIRE(cond)                                                                  \
    do                                                                                 \
    {                                                                                  \
        if (!(cond))                                                                   \
        {                                                                              \
            fprintf(stderr, "%s:%d: REQUIRE failed: %s\n", __FILE__, __LINE__, #cond); \
            return 1;                                                                  \
        }                                                                              \
    } while (0)

#define CRIT_ERR(msg)                                                      \
    do                                                                     \
    {                                                                      \
        fprintf(stderr, "CRIT ERR: %s:%d: %s\n", __FILE__, __LINE__, msg); \
    } while (0)

static int testCount = 0;
static int passed = 0;

TDataArena *globalArena = NULL;

static void *xmalloc(size_t size)
{
    if (globalArena == NULL)
    {
        CRIT_ERR("No data arena");
        return NULL;
    }
    void *block = arena_malloc(globalArena, size);
    if (block == NULL)
        CRIT_ERR("Memory limit exceded");

    return block;
}

static char *str_copy(const char *s)
{
    if (!s)
        return NULL;
    size_t n = strlen(s) + 1;
    char *p = (char *)xmalloc(n);
    if (!p)
        return NULL;
    memcpy(p, s, n);
    return p;
}

static TToken make_token(int type, const char *s)
{
    if (s == NULL)
    {
        TToken t = {.type = ERROR_TOKEN, .start = NULL, .end = NULL, .col = 0, .lineno = 0};
        return t;
    }
    TToken t;
    t.col = 0;
    t.lineno = 0;
    t.type = (TokenTypes)type;
    t.start = s;
    t.end = s ? s + strlen(s) : NULL;
    return t;
}

static TToken make_index_token(int index)
{
    size_t maxStr = 16;
    char *tokenStr = (char *)xmalloc(maxStr);
    if (tokenStr != NULL)
        snprintf(tokenStr, maxStr, "%d", index);
    return make_token(IDENT, tokenStr);
}

static int check_token(TTokenBuffer *buf, int pos, int expected_type, const char *str)
{
    TToken t;
    REQUIRE(get_token_from_buf(buf, &t, pos) == 1);
    REQUIRE((int)t.type == expected_type);
    if (str != NULL)
    {
        REQUIRE(t.start != NULL);
        REQUIRE(check_token_str(t, str) == 1);
    }
    return 0;
}

/* 1. Append + get basic */
static int test_append_and_get(TTokenBuffer *buf)
{
    TToken a = make_token(1, "one");
    TToken b = make_token(2, "two");

    REQUIRE(append_token_to_buf(buf, a) == 1);
    REQUIRE(get_buf_length(buf) == 1);
    REQUIRE(append_token_to_buf(buf, b) == 1);
    REQUIRE(get_buf_length(buf) == 2);

    check_token(buf, 0, 1, "one");
    check_token(buf, 1, 2, "two");

    printf("test_append_and_get: OK\n");
    return 0;
}

/* 2. Growth: использует make_index_token */
static int test_growth_preserves_data(TTokenBuffer *buf)
{
    char expected[32];
    const int N = 1000; // force grow
    for (int i = 0; i < N; ++i)
    {
        TToken t = make_index_token(i);
        REQUIRE(t.type != ERROR_TOKEN);
        REQUIRE(append_token_to_buf(buf, t) == 1);
        if (i < 10)
            printf("%s", t.start);
    }
    REQUIRE(get_buf_length(buf) == N);

    TToken got;
    REQUIRE(get_token_from_buf(buf, &got, 0) == 1);
    printf("%s\n", got.start);
    REQUIRE(check_token_str(got, "0"));

    REQUIRE(get_token_from_buf(buf, &got, N - 1) == 1);

    snprintf(expected, sizeof(expected), "%d", N - 1);
    REQUIRE(check_token_str(got, expected));

    REQUIRE(get_token_from_buf(buf, &got, N / 2) == 1);

    snprintf(expected, sizeof(expected), "%d", N / 2);
    REQUIRE(check_token_str(got, expected));

    printf("test_growth_preserves_data: OK\n");
    return 0;
}

/* 3. Wrap-around behavior */
static int test_wraparound_and_order(TTokenBuffer *buf)
{
    char name[16];
    // append A..J (values 0..9)
    for (int i = 0; i < 10; ++i)
    {
        snprintf(name, sizeof(name), "v%d", i);
        char *s = str_copy(name);
        REQUIRE(append_token_to_buf(buf, make_token(i, s)) == 1);
    }
    REQUIRE(get_buf_length(buf) == 10);

    // pop 7 -> remaining should be 7..9 (3 items)
    REQUIRE(pop_tokens_from_buf(buf, 7) == 1);
    REQUIRE(get_buf_length(buf) == 3);

    // append K..T (values 10..19)
    for (int i = 10; i < 20; ++i)
    {
        snprintf(name, sizeof(name), "v%d", i);
        char *s = str_copy(name);
        REQUIRE(append_token_to_buf(buf, make_token(i, s)) == 1);
    }

    // Now logical sequence: 7,8,9,10..19  => total 13
    REQUIRE(get_buf_length(buf) == 13);

    check_token(buf, 0, 7, "v7");
    check_token(buf, 3, 10, "v10");
    check_token(buf, 12, 19, "v19");

    printf("test_wraparound_and_order: OK\n");
    return 0;
}

/* 4. pop_tokens_from_buf full pop */
static int test_pop_tokens_full_and_empty(TTokenBuffer *buf)
{
    for (int i = 0; i < 5; ++i)
        REQUIRE(append_token_to_buf(buf, make_token(i, NULL)) == 1);

    REQUIRE(get_buf_length(buf) == 5);

    // pop all
    REQUIRE(pop_tokens_from_buf(buf, 5) == 1);
    REQUIRE(get_buf_length(buf) == 0);

    // get out of range
    TToken t;
    REQUIRE(get_token_from_buf(buf, &t, 0) == 0);

    printf("test_pop_tokens_full_and_empty: OK\n");
    return 0;
}

/* 5. get out-of-range check */
static int test_get_out_of_range(TTokenBuffer *buf)
{
    // append one
    REQUIRE(append_token_to_buf(buf, make_token(1, "one")) == 1);
    REQUIRE(get_buf_length(buf) == 1);

    TToken tmp;
    REQUIRE(get_token_from_buf(buf, &tmp, 0) == 1);
    REQUIRE(get_token_from_buf(buf, &tmp, 1) == 0); // out of range

    printf("test_get_out_of_range: OK\n");
    return 0;
}

static void test_run(int (*test)(TTokenBuffer *buf))
{
    TTokenBuffer *buf = get_token_buf();
    if (!buf)
    {
        CRIT_ERR("Failed to allocate buffer\n");
        return;
    }

    testCount++;
    int r = test(buf);
    if (r == 0)
    {
        passed++;
    }
    else
    {
        fprintf(stderr, "FAILED TEST #%d\n", testCount);
    }

    delete_token_buf(buf);
}

void run_buffer_tests()
{
    globalArena = get_data_arena();

    printf("=== token_buffer tests start ===\n");
    test_run(test_append_and_get);
    test_run(test_growth_preserves_data);
    test_run(test_wraparound_and_order);
    test_run(test_pop_tokens_full_and_empty);
    test_run(test_get_out_of_range);
    printf("=== token_buffer tests end: %d/%d PASSED ===\n", passed, testCount);

    delete_arena(globalArena);
    globalArena = NULL;
}
