#include "../header/lexer.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

const char *keywords[] = {
    "exit", "return", "main", "class", "if", "else", "ifelse", "while", "do", "break"
};

const char* getTokenTypeString(Token token) {
    switch (token.type) {
        case SEPARATOR:
            return "SEPARATOR";
        case KEYWORD:
            return "KEYWORD";
        case OPERATION:
            return "OPERATION";
        case NUMERIC:
            return "NUMERIC";
        default:
            return "UNKNOWN";
    }
}

Token readFullWord(char current_char, FILE *file) {
    Token token;
    token.type = KEYWORD;
    token.separatorType = CHARACTER;
    token.word = (char *)malloc(sizeof(char) * 2);
    int word_index = 0;
    int buffer_size = 2;

    token.word[word_index++] = current_char;

    while (isalpha((current_char = fgetc(file))) || current_char == '_') {
        if (word_index >= buffer_size - 1) {
            buffer_size *= 2;
            token.word = (char *)realloc(token.word, buffer_size * sizeof(char));
            if (!token.word) {
                fprintf(stderr, "Error: Memory allocation failed\n");
                exit(EXIT_FAILURE);
            }
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
    token.type = SEPARATOR;
    token.separatorType = STRING_LITERAL;
    token.word = (char *)malloc(sizeof(char) * 2);
    if (!token.word) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    int index = 0;
    int buffer_size = 2;

    char current_char = fgetc(file);

    while (current_char != '"' && current_char != EOF) {
        if (index >= buffer_size - 1) {
            buffer_size *= 2;
            token.word = (char *)realloc(token.word, buffer_size * sizeof(char));
            if (!token.word) {
                fprintf(stderr, "Error: Memory allocation failed\n");
                exit(EXIT_FAILURE);
            }
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
    token.type = NUMERIC;
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
        Token token;
        if (current_char == ';') {
            printf("found Semicolon, type: %s\n", getTokenTypeString((Token){SEPARATOR, SEMI}));
        } else if (current_char == '(') {
            printf("found open parenthesis, type: %s\n", getTokenTypeString((Token){SEPARATOR, OPEN_PARENTHESIS}));
        } else if (current_char == ')') {
            printf("found closed parenthesis, type: %s\n", getTokenTypeString((Token){SEPARATOR, CLOSED_PARENTHESIS}));
        } else if (current_char == '{') {
            printf("found open brace, type: %s\n", getTokenTypeString((Token){SEPARATOR, OPEN_BRACE}));
        } else if (current_char == '}') {
            printf("found closed brace, type: %s\n", getTokenTypeString((Token){SEPARATOR, CLOSED_BRACE}));
        } else if (current_char == '[') {
            printf("found open bracket, type: %s\n", getTokenTypeString((Token){SEPARATOR, OPEN_BRACKET}));
        } else if (current_char == ']') {
            printf("found closed bracket, type: %s\n", getTokenTypeString((Token){SEPARATOR, CLOSED_BRACKET}));
        } else if (current_char == ',') {
            printf("found comma, type: %s\n", getTokenTypeString((Token){SEPARATOR, COMMA}));
        } else if (current_char == '.') {
            printf("found dot, type: %s\n", getTokenTypeString((Token){SEPARATOR, DOT}));
        } else if (current_char == ':') {
            printf("found colon, type: %s\n", getTokenTypeString((Token){SEPARATOR, COLON}));
        } else if (current_char == '+') {
            printf("found addition, type: %s\n", getTokenTypeString((Token){OPERATION, ADDITION}));
        } else if (current_char == '-') {
            printf("found subtraction, type: %s\n", getTokenTypeString((Token){OPERATION, SUBTRACTION}));
        } else if (current_char == '*') {
            printf("found multiplication, type: %s\n", getTokenTypeString((Token){OPERATION, MULTIPLICATION}));
        } else if (current_char == '/') {
            char next_char = fgetc(file);
            if (next_char == '/') {
                while (current_char != '\n' && current_char != EOF) {
                    current_char = fgetc(file);
                }
                continue;
            } else {
                fseek(file, -1, SEEK_CUR);
                printf("found division, type: %s\n", getTokenTypeString((Token){OPERATION, DIVISION}));
            }
        } else if (current_char == '>') {
            printf("found greater than, type: %s\n", getTokenTypeString((Token){OPERATION, GREATER_THAN}));
        } else if (current_char == '<') {
            printf("found less than, type: %s\n", getTokenTypeString((Token){OPERATION, LESS_THAN}));
        } else if (current_char == '=') {
            printf("found equal to, type: %s\n", getTokenTypeString((Token){OPERATION, EQUAL_TO}));
        } else if (current_char == '!') {
            printf("found not equal to, type: %s\n", getTokenTypeString((Token){OPERATION, NOT_EQUAL_TO}));
        } else if (isalpha(current_char) || current_char == '_') {
            Token token_char = readFullWord(current_char, file);
            printf("identifier/keyword: %s, type: %s\n", token_char.word, getTokenTypeString(token_char));
            free(token_char.word);
        } else if (isdigit(current_char) || current_char == '.') {
            fseek(file, -1, SEEK_CUR);
            Token token = readNumber(file);
            if (token.numericType == INTEGER) {
                printf("integer value %d, type: %s\n", token.value.intValue, getTokenTypeString(token));
            } else if (token.numericType == FLOAT) {
                printf("float value %f, type: %s\n", token.value.floatValue, getTokenTypeString(token));
            }
        } else if (current_char == '"') {
            Token token = readStringLiteral(file);
            if (token.word != NULL) {
                printf("string literal: %s, type: %s\n", token.word, getTokenTypeString(token));
                free(token.word);
            }
        } else {
            printf("found unknown character: %c\n", current_char);
        }
    }
}