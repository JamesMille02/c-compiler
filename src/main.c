#include "../header/lexer.h"

int main() {
    FILE *file = fopen("jlang/code.jlang", "r");
    if (!file) {
        fprintf(stderr, "Error opening file\n");
        return 1;
    }
    lexer(file);
    fclose(file);
    return 0;
}
