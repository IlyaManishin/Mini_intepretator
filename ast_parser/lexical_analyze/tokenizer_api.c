#include <assert.h>

#include "tokenizer_api.h"
#include "token_buffer.h"
#include "tokenizer.h"

TTokenizer *tokenizer_from_file_data(TFileData fileData)
{
    assert(fileData.str != NULL);

    TTokenBuffer *tokBuffer = get_token_buf();
    if (tokBuffer == NULL)
        return NULL;

    TTokenizer *tokenizer = (TTokenizer *)malloc(sizeof(TTokenizer));
    if (tokenizer == NULL)
    {
        delete_token_buf(tokBuffer);
        return NULL;
    }

    tokenizer->start = fileData.str;
    tokenizer->cur = fileData.str;
    tokenizer->curLine = fileData.str;
    tokenizer->lineIndex = 0;
    tokenizer->end = fileData.str + fileData.dataSize;

    tokenizer->state = NEW_LINE_STATE;
    tokenizer->newIndent = 0;
    tokenizer->curIndent = 0;

    tokenizer->tokensBuf = tokBuffer;

    tokenizer->isError = false;
    tokenizer->tokError.textMsg = NULL;

    return tokenizer;
}

void delete_tokenizer(TTokenizer *tokenizer)
{
    free(tokenizer->tokensBuf);
    free(tokenizer);
}

bool is_tokenizer_error(TTokenizer *tokenizer)
{
    return tokenizer->isError;
}

void pass_tokenizer_error(TTokenizer *tokenizer)
{
    tokenizer->isError = false;
}

TTokenizerError get_tokenizer_error(TTokenizer *tokenizer)
{
    return tokenizer->tokError;
}

// TToken get_token(TTokenizer *tokenizer)
// {
//     if (get_token_buffer_length(tokenizer->tokensBuf) == 0)
//     {
//         return read_new_token(tokenizer);
//     }
//     return pop_token_from_buf(tokenizer->tokensBuf);
// }

// void back_token(TTokenizer *tokenizer, TToken token)
// {
//     if (is_tokenizer_error(tokenizer))
//         return;

//     int r = append_token(tokenizer->tokensBuf, token);
//     if (r == 0)
//     {
//         set_base_tokenizer_error(tokenizer, "Memory limit exceded");
//     }
// }

TToken soft_token_read(TTokenizer *tokenizer)
{
    TToken token;
    if (get_token_from_buf(tokenizer->tokensBuf, &token))
    {
        return token;
    }

    token = read_new_token(tokenizer);
    int r = append_token_to_buf(tokenizer->tokensBuf, token);
    if (!r)
    {
        set_memory_error(tokenizer);
        TToken errorToken = make_error_token(tokenizer);
        return errorToken;
    }
    return token;
}

TToken strong_token_read(TTokenizer *tokenizer)
{
    TToken token;
    if (pop_token_from_buf(tokenizer->tokensBuf, &token))
    {
        return token;
    }
    token = read_new_token(tokenizer);
    return token;
}

