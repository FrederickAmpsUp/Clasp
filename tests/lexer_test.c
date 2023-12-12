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
    str = (StringStream) { "5wowo 76+82 e68==75= -> 7 %%= %= 5%5", 0 };
    ClaspLexer *l = malloc(sizeof(ClaspLexer));
    new_lexer(l, read_string, NULL);

    ClaspToken *current;

    ClaspTokenType target_type[] = {
        TOKEN_NUMBER, TOKEN_ID, TOKEN_NUMBER, TOKEN_PLUS, TOKEN_NUMBER, TOKEN_ID, TOKEN_EQ_EQ, TOKEN_NUMBER, TOKEN_EQ, TOKEN_RIGHT_POINT, TOKEN_NUMBER,
        TOKEN_PERC,TOKEN_PERC_EQ,TOKEN_PERC_EQ,TOKEN_NUMBER, TOKEN_PERC, TOKEN_NUMBER
    };

    current = lexer_next(l);
    unsigned int i = 0;
    while (current->type != TOKEN_EOF &&  current->type != TOKEN_UNKNOWN) {
        token_print(current);
        assert(current->type == target_type[i]);
        current = lexer_next(l);
        ++i;
    }

    token_print(current);
    
    return 0;
}