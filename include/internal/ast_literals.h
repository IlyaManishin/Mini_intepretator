#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

typedef uint8_t digit;

typedef enum LiteralTypes
{
    STRING_L,
    NUMBER_L
} LiteralTypes;

typedef struct TLong
{
    digit *digits;
    int digitsCount;
    int sign;
} TLong;

typedef struct TNumber
{
    bool isLong;
    union
    {
        long long intData;
        TLong longData;
    } numData;

} TNumber;

typedef struct TString
{
    char *data;
    size_t length;
} TString;

typedef struct TLiteral
{
    LiteralTypes type;
    union
    {
        TNumber number;
        TString string;
    };

} TLiteral;