#include <clasp/lexer.h>
#include <clasp/parser.h>
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
    str = (StringStream) { "3 + 5 - 2 * 8", 0 };
    ClaspLexer *l = malloc(sizeof(ClaspLexer));
    new_lexer(l, read_string, NULL);
    ClaspParser *p = malloc(sizeof(ClaspParser));
    FILE *out = fopen("a.out", "wb");
    if (!out) {
        fprintf(stderr, "Failed to open file a.out\n");
        return -1;
    }
    new_parser(p, l, out);
    parser_compile(p);

    
    return 0;
}