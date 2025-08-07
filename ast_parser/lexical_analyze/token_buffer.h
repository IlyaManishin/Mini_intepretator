#pragma once

#include "tokenizer_api.h"

#define BASE_SIZE 128

TTokBuffer *get_token_buffer();
void delete_token_buffer(TTokBuffer *buffer);

int append_token(TTokBuffer *buffer, TToken token);
TToken pop_token_from_buffer(TTokBuffer *buffer);

int get_token_buffer_length(TTokBuffer *buffer);
