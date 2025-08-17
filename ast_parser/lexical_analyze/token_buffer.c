#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include "token_buffer.h"

#define abs(a) ((a) >= 0) ? a : -a
#define to_positive(a) (a < 0 ? 0 : a)

TTokenBuffer *get_token_buf()
{
    TTokenBuffer *buf = (TTokenBuffer *)malloc(sizeof(TTokenBuffer));
    if (buf == NULL)
        return NULL;

    buf->data = (TToken *)malloc(BASE_SIZE * sizeof(TToken));
    if (buf->data == NULL)
    {
        free(buf);
        return NULL;
    }
    buf->curPos = 0;
    buf->left = 0;
    buf->right = 0;
    buf->capacity = BASE_SIZE;
    return buf;
}

static int get_buf_length(TTokenBuffer *buf)
{
    int delta = buf->right - buf->left;
    return delta >= 0 ? delta : delta + buf->capacity;
}

static int buf_mod(TTokenBuffer *buf, int value)
{
    return value % buf->capacity;
}

static int increase_buf(TTokenBuffer *buf)
{
    int new_capacity = buf->capacity * 2;
    buf->data = (TToken *)realloc(buf->data, sizeof(TToken) * new_capacity);
    if (buf->data == NULL)
        return 0;
    if (buf->left >= buf->right)
    {
        for (int i = 0; i < buf->right; i++)
        {
            buf->data[buf->capacity + i] = buf->data[i];
        }
        buf->right = buf->capacity + buf->right;
    }
    buf->capacity = new_capacity;
    return 1;
}

int append_token_to_buf(TTokenBuffer *buf, TToken token)
{
    assert(buf->curPos == buf->right);

    if (get_buf_length(buf) == buf->capacity - 1)
    {
        int r = increase_buf(buf);
        if (!r)
            return 0;
    }
    buf->data[buf->right] = token;
    buf->right = buf_mod(buf, buf->right + 1);
    buf->curPos = buf->right;
    return 1;
}

int pop_token_from_buf(TTokenBuffer *buf, TToken *dest)
{
    if (get_buf_length(buf) == 0)
        return 0;

    *dest = buf->data[buf->left];
    buf->left = buf_mod(buf, buf->left + 1);
    buf->curPos = to_positive(buf->curPos - 1);
    return 1;
}

int get_token_from_buf(TTokenBuffer *buf, TToken *dest)
{
    if (buf->curPos == get_buf_length(buf))
        return 0;

    int index = buf_mod(buf, buf->left + buf->curPos);
    *dest = buf->data[index];
    buf->curPos++;
    return 1;
}

void set_buf_pos(TTokenBuffer *buf, int newPos)
{
    assert(0 <= newPos && newPos < get_buf_length(buf));
    buf->curPos = newPos;
}

int get_buf_pos(TTokenBuffer *buf)
{
    return buf->curPos;
}

void rewind_buf_pos(TTokenBuffer *buf)
{
    buf->curPos = 0;
}

void delete_token_buf(TTokenBuffer *buf)
{
    if (buf == NULL)
        return;

    free(buf->data);
    free(buf);
}