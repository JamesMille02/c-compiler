#include <stdio.h>
#include "lexer.h"

int main() {
    FILE *file = fopen("jlang/code.jlang", "r");
    if (!file) {
        perror("Failed to open file");
        return EXIT_FAILURE;
    }

    Token *tokens = lexer(file);
    
    for (size_t i = 0; tokens[i].type != END_OF_TOKENS; i++) {
        print_token(&tokens[i]);  
        free_token(&tokens[i]);   
    }

    free(tokens);
    return EXIT_SUCCESS;
}
