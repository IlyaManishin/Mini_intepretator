#pragma once

#include "token.h"

#define BASE_SIZE 128

typedef struct TTokenBuffer
{
    TToken *data;
    int left, right;
    int capacity;
} TTokenBuffer;

TTokenBuffer *get_token_buf();
void delete_token_buf(TTokenBuffer *buf);

int append_token_to_buf(TTokenBuffer *buf, TToken token);
int pop_tokens_from_buf(TTokenBuffer *buf, int tokensCount);
int get_token_from_buf(TTokenBuffer *buf, TToken *dest, int curPos);
int get_buf_length(TTokenBuffer *buf);

