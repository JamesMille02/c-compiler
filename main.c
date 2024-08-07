#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

typedef enum {
    SEMI,
    OPEN_PARENTHESIS,
    CLOSED_PARENTHESIS,
    OPEN_BRACE,
    CLOSED_BRACE,
    CHARACTER,
    STRING_LITERAL
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
    EQUAL_TO
} TypeOperations;

typedef struct {
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

const char *keywords[] = {
    "exit", "return", "main", "class", "if", "else", "ifelse", "while", "do", "break"
};
#define NUM_KEYWORDS (sizeof(keywords) / sizeof(keywords[0]))

Token readFullWord(char current_char, FILE *file);
Token readStringLiteral(FILE *file);
Token readNumber(FILE *file);

Token readFullWord(char current_char, FILE *file) {
    Token token;
    token.separatorType = CHARACTER;
    token.word = (char *)malloc(sizeof(char) * 2);
    int word_index = 0;
    int buffer_size = 2;

    token.word[word_index++] = current_char;

    while (isalpha((current_char = fgetc(file))) && current_char != EOF) {
        if (word_index >= buffer_size - 1) {
            buffer_size *= 2;
            token.word = (char *)realloc(token.word, buffer_size * sizeof(char));
        }
        token.word[word_index++] = current_char;
    }
    token.word[word_index] = '\0';

    if (current_char != EOF) {
        fseek(file, -1, SEEK_CUR);
    }

    token.keywordType = UNKNOWN_KEYWORD;
    for (int i = 0; i < NUM_KEYWORDS; i++) {
        if (strcmp(token.word, keywords[i]) == 0) {
            token.keywordType = (TypeKeyword)i;
            break;
        }
    }

    if (token.keywordType == UNKNOWN_KEYWORD) {
        fprintf(stderr, "Error: Unknown keyword '%s'\n", token.word);
        free(token.word);
        exit(EXIT_FAILURE);
    }

    return token;
}

Token readStringLiteral(FILE *file) {
    Token token;
    token.separatorType = STRING_LITERAL;
    token.word = (char *)malloc(sizeof(char) * 2);
    int index = 0;
    int buffer_size = 2;

    char current_char = fgetc(file);

    while (current_char != '"' && current_char != EOF) {
        if (index >= buffer_size - 1) {
            buffer_size *= 2;
            token.word = (char *)realloc(token.word, buffer_size * sizeof(char));
        }
        token.word[index++] = current_char;
        current_char = fgetc(file);
    }
    token.word[index] = '\0';

    if (current_char != '"') {
        fprintf(stderr, "Error: Missing a quote\n");
        free(token.word);
        token.word = NULL;
    }

    return token;
}

Token readNumber(FILE *file) {
    Token token;
    token.word = NULL;

    char current_char;
    char buffer[256];
    int index = 0;
    int buffer_size = sizeof(buffer);
    
    while (isdigit((current_char = fgetc(file))) || current_char == '.') {
        if (index >= buffer_size - 1) {
            fprintf(stderr, "Error: Number exceeds buffer size\n");
            exit(EXIT_FAILURE);
        }
        buffer[index++] = current_char;
    }

    if (current_char != EOF) {
        fseek(file, -1, SEEK_CUR);
    }

    buffer[index] = '\0';

    if (strchr(buffer, '.') != NULL) {
        token.numericType = FLOAT;
        token.value.floatValue = strtof(buffer, NULL);
    } else {
        token.numericType = INTEGER;
        token.value.intValue = atoi(buffer);
    }

    return token;
}

void lexer(FILE *file) {
    char current_char = fgetc(file);
    
    while (current_char != EOF) {
        if (current_char == ';') {
            printf("found Semicolon\n");
        } else if (current_char == '(') {
            printf("found open par\n");
        } else if (current_char == ')') {
            printf("found closed par\n");
        } else if (isalpha(current_char)) {
            Token token_char = readFullWord(current_char, file);
            printf("is a keyword: %s\n", token_char.word);
            free(token_char.word); 
        } else if (isdigit(current_char) || current_char == '.') {
            fseek(file, -1, SEEK_CUR); 
            Token token = readNumber(file);
            if (token.numericType == INTEGER) {
                printf("integer value %d\n", token.value.intValue);
            } else if (token.numericType == FLOAT) {
                printf("float value %f\n", token.value.floatValue);
            }
        } else if (current_char == '"') {
            Token token_str = readStringLiteral(file);
            if (token_str.word != NULL) {
                printf("string literal: %s\n", token_str.word);
                free(token_str.word);
            }
        }
        current_char = fgetc(file);
    }
}

int main() {
    FILE *file;
    file = fopen("code.jlang", "r");
    if (!file) {
        fprintf(stderr, "Error opening file\n");
        return 1;
    }
    lexer(file);
    fclose(file);
    return 0;
}
