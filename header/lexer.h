#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>

typedef enum {
    SEMI,
    OPEN_PARENTHESIS,
    CLOSED_PARENTHESIS,
    OPEN_BRACE,
    CLOSED_BRACE,
    OPEN_BRACKET,
    CLOSED_BRACKET,
    COMMA,
    DOT,
    COLON,
    STRING_LITERAL,
    CHARACTER,
    UNKNOWN
} TypeSeparators;

typedef enum {
    INTEGER,
    FLOAT
} TypeNumeric;

typedef enum {
    EXIT,
    RETURN,
    MAIN,
    CLASS,
    IF,
    ELSE,
    IFELSE,
    WHILE,
    DO,
    BREAK,
    UNKNOWN_KEYWORD
} TypeKeyword;

typedef enum {
    ADDITION,
    SUBTRACTION,
    MULTIPLICATION,
    DIVISION,
    GREATER_THAN,
    LESS_THAN,
    EQUAL_TO,
    NOT_EQUAL_TO,
    ASSIGNMENT,
    AND,
    OR
} TypeOperations;

typedef enum {
    SEPARATOR,
    KEYWORD,
    OPERATION,
    NUMERIC
} TokenType;

typedef struct {
    TokenType type;
    TypeSeparators separatorType;
    TypeKeyword keywordType;
    TypeOperations operationType;
    TypeNumeric numericType;
    union {
        int intValue;
        float floatValue;
    } value;
    char *word;
} Token;

extern const char *keywords[];
#define NUM_KEYWORDS (10)

Token readFullWord(char current_char, FILE *file);
Token readStringLiteral(FILE *file);
Token readNumber(FILE *file);
void lexer(FILE *file);
const char* getTokenTypeString(Token token);

#endif 
