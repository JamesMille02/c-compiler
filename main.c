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
    EXIT,
} TypeKeyword;

typedef enum {
    INT,
} TypeLiteral;

typedef struct {
    TypeKeyword keywordType;
    TypeSeparator separatorType;
    TypeLiteral literalType;
    int value;
    char character;
} Token;



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
            printf("is a character \n");
        } else if (isdigit(current_char)) {
            char s1[20] ={0};
            int index = 0;
            
            while(isdigit(current_char) && index < sizeof(s1) -1) {
                s1[index++] = current_char;
                current_char = fgetc(file);
            }
            s1[index] = '\0';
            int combined_int = atoi(s1);
            printf("combined digit: %d\n", combined_int);

            if (!isdigit(current_char)) {
                fseek(file, -1, SEEK_CUR);
            }  
        } else {
        }
        current_char = fgetc(file);
    }

}

int main(){
    FILE *file;
    file = fopen("code.jlang", "r");
    lexer(file);
}
