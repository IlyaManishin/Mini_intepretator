#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include "token_buffer.h"

TTokBuffer *get_token_buffer()
{
    TTokBuffer *buffer = (TTokBuffer *)malloc(sizeof(TTokBuffer));
    if (buffer == NULL)
        return NULL;

    buffer->data = (TToken *)malloc(BASE_SIZE * sizeof(TToken));
    if (buffer->data == NULL)
    {
        free(buffer);
        return NULL;
    }

    buffer->length = 0;
    buffer->capacity = BASE_SIZE;
    return buffer;
}

int append_token(TTokBuffer *buffer, TToken token)
{
    if (buffer->length >= buffer->capacity)
    {
        int new_capacity = buffer->capacity * 2;
        TToken *data = (TToken *)realloc(buffer->data, sizeof(TToken) * new_capacity);
        if (data == NULL)
            return 0;

        buffer->data = data;
        buffer->capacity = new_capacity;
    }

    buffer->data[buffer->length++] = token;
    return 1;
}

TToken pop_token_from_buffer(TTokBuffer *buffer)
{
    assert(buffer->length != 0);
    return buffer->data[--buffer->length];
}

int get_token_buffer_length(TTokBuffer *buffer)
{
    return buffer->length;
}

void delete_token_buffer(TTokBuffer *buffer)
{
    if (buffer == NULL)
        return;
    assert(buffer->data != NULL);

    free(buffer->data);
    free(buffer);
}