#include "../header/lexer.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

const char *keywords[] = {
    "exit", "return", "main", "class", "if", "else", "ifelse", "while", "do", "break"
};

Token readFullWord(char current_char, FILE *file) {
    Token token;
    token.separatorType = CHARACTER;
    token.word = (char *)malloc(sizeof(char) * 2);
    int word_index = 0;
    int buffer_size = 2;

    token.word[word_index++] = current_char;

    while (isalpha((current_char = fgetc(file))) || current_char == '_') {
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
        fprintf(stderr, "Error: Missing closing quote\n");
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
    char current_char;
    
    while ((current_char = fgetc(file)) != EOF) {
        if (current_char == ';') {
            printf("found Semicolon\n");
        } else if (current_char == '(') {
            printf("found open parenthesis\n");
        } else if (current_char == ')') {
            printf("found closed parenthesis\n");
        } else if (current_char == '{') {
            printf("found open brace\n");
        } else if (current_char == '}') {
            printf("found closed brace\n");
        } else if (current_char == '[') {
            printf("found open bracket\n");
        } else if (current_char == ']') {
            printf("found closed bracket\n");
        } else if (current_char == ',') {
            printf("found comma\n");
        } else if (current_char == '.') {
            printf("found dot\n");
        } else if (current_char == ':') {
            printf("found colon\n");
        } else if (current_char == '+') {
            printf("found addition\n");
        } else if (current_char == '-') {
            printf("found subtraction\n");
        } else if (current_char == '*') {
            printf("found multiplication\n");
        } else if (current_char == '/') {
            char next_char = fgetc(file);
            if (next_char == '/') {
                while (current_char != '\n' && current_char != EOF) {
                    current_char = fgetc(file);
                }
                continue;
            } else {
                fseek(file, -1, SEEK_CUR);
                printf("found division\n");
            }
        } else if (current_char == '>') {
            printf("found greater than\n");
        } else if (current_char == '<') {
            printf("found less than\n");
        } else if (current_char == '=') {
            printf("found equal to\n");
        } else if (current_char == '!') {
            printf("found not equal to\n");
        } else if (isalpha(current_char) || current_char == '_') {
            Token token_char = readFullWord(current_char, file);
            printf("identifier/keyword: %s\n", token_char.word);
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
        } else if (isspace(current_char)) {
            continue;
        } else {
            printf("Unknown character: %c\n", current_char);
        }
    }
}
