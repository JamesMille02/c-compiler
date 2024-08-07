#include <stdio.h>
#include <stdlib.h>

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
} Token;

int main(){
    FILE *file;
    file = fopen("code.jlang", "r");
    char current = fgetc(file);

    while(current!= EOF){
        printf("%c", current);
        current = fgetc(file);
    }
}
