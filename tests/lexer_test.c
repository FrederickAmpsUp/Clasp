#include <clasp/lexer.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef struct {
    char *data;
    unsigned int idx;
} StringStream;

StringStream str;

char read_string() {
    if (str.idx == strlen(str.data)) return EOF;
    return str.data[str.idx++];
}

int main(int argc, char **argv) {
    str = (StringStream) { "5wowo 76+82 e68==75= -> 7", 0 };
    ClaspLexer *l = malloc(sizeof(ClaspLexer));
    new_lexer(l, read_string);

    ClaspToken *current;

    current = lexer_next(l);
    while (current->type != TOKEN_EOF &&  current->type != TOKEN_UNKNOWN) {
        token_print(current);
        current = lexer_next(l);
    }

    token_print(current);
    
    return 0;
}