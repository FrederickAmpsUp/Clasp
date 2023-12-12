#include <clasp/lexer.h>
#include <clasp/stringstream.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

int main(int argc, char **argv) {
    StringStream *str = new_stream("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890  \n\nabcd");
    
    char c;
    while ((c = stream_read(str)) != EOF) {
        putchar(c);
    }
    putchar('\n');

    return 0;
}