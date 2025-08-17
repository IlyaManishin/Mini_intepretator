#pragma once

#include "token_buffer.h"
#include "tokenizer_api.h"

#define BASE_SIZE 128

typedef struct TTokenBuffer
{
    TToken *data;
    int curPos; // offset from left, need for lookahead
    int left, right;
    int capacity;
} TTokenBuffer;

TTokenBuffer *get_token_buf();
void delete_token_buf(TTokenBuffer *buf);

int append_token_to_buf(TTokenBuffer *buf, TToken token);
int pop_token_from_buf(TTokenBuffer *buf, TToken *dest);
int get_token_from_buf(TTokenBuffer *buf, TToken *dest);

void set_buf_pos(TTokenBuffer *buf, int newPos);
int get_buf_pos(TTokenBuffer *buf);
void rewind_buf_pos(TTokenBuffer *buf);


