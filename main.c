#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

typedef enum {
    SEMI,
    OPEN_PARENTHESIS,
    CLOSED_PARENTHESIS
} TypeSeparator;

typedef enum{
    CHARACTER,
} TypeKeyword;

typedef enum {
    INT,
} TypeInteger;


typedef struct{
    TypeSeparator separatorType;
    TypeInteger integerType;
    TypeKeyword keywordType;
    int value;
    char character;
    char word[256];
} Token;

Token readFullNumber(char current_char, FILE *file) {
    Token token;
    token.integerType = INT;
    int value = current_char - '0';
    while (isdigit((current_char = fgetc(file))) && current_char != EOF) {
        value = value * 10 + (current_char - '0');
    }
    if (current_char != EOF) {
        fseek(file, -1, SEEK_CUR);
    }
    token.value = value;
    return token;
}

Token readFullWord(char current_char, FILE *file) {
    Token token;
    token.keywordType = CHARACTER;
    int word_index = 0;

    token.word[word_index++] = current_char;

    while (isalpha((current_char = fgetc(file))) && current_char != EOF) {
        if(word_index < sizeof(token.word) -1){
            token.word[word_index++] = current_char;
        }
    }
    token.word[word_index] = '\0';
    if (current_char != EOF) {
        fseek(file, -1, SEEK_CUR);
    }
    return token;
}


void lexer(FILE *file){
    char current_char = fgetc(file);
    
    while(current_char != EOF){
        if(current_char == ';') {
            printf("found Semicolon \n");
        } else if (current_char == '(') {
            printf("found open par \n");
        } else if (current_char == ')') {
            printf("found closed par \n");
        } else if (isalpha(current_char)) {
            Token token = readFullWord(current_char, file);
            printf("is a word: %s\n", token.word);
        } else if (isdigit(current_char)) {
            Token token_int_test = readFullNumber(current_char, file);
            printf("token value %d\n", token_int_test.value);
        }
        current_char = fgetc(file);
    }

}

int main(){
    FILE *file;
    file = fopen("code.jlang", "r");
    lexer(file);
}
