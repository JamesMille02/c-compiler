#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

typedef enum {
    SEMI,
    OPEN_PARENTHESIS,
    CLOSED_PARENTHESIS,
    OPEN_BRACE,
    CLOSED_BRACE,
    CHARACTER,
    STRING_LITERAL,
    SINGLE_QUOTE
} TypeSeparators;

typedef enum {
    INTEGER
} TypeInteger;

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
    TypeInteger integerType;
    int value;
    char *word;
} Token;

const char *keywords[] = {
    "exit", "return", "main", "class", "if", "else", "ifelse", "while", "do", "break"
};
#define NUM_KEYWORDS (sizeof(keywords) / sizeof(keywords[0]))

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
        fprintf(stderr, "Error: Unknown keyword: '%s'\n", token.word);
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
        fprintf(stderr, "Error: Unclosed string literal\n");
    }

    return token;
}

Token readFullNumber(char current_char, FILE *file) {
    Token token;
    token.integerType = INTEGER;
    int value = 0;
    value = current_char - '0';
    while (isdigit((current_char = fgetc(file))) && current_char != EOF) {
        value = value * 10 + (current_char - '0');
    }
    if (current_char != EOF) {
        fseek(file, -1, SEEK_CUR);
    }
    token.value = value;
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
        } else if (isdigit(current_char)) {
            Token token_int = readFullNumber(current_char, file);
            printf("token value %d\n", token_int.value);
        } else if (current_char == '"') {
            Token token_str = readStringLiteral(file);
            if (token_str != "error")
            printf("string literal: %s\n", token_str.word);
            free(token_str.word); 
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
