#include <clasp/lexer.h>
#include <clasp/stringstream.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

int main(int argc, char **argv) {
    StringStream str = (StringStream) { "5wowo 76+82 e68==75= -> 7", 0 };
    
    char c;
    while ((c = stream_read(&str)) != EOF) {
        putchar(c);
    }
    putchar('\n');

    return 0;
}