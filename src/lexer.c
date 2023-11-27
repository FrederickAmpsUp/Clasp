#include <clasp/lexer.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

void new_lexer(ClaspLexer *lexer, StreamReadFn fn) {
    lexer->stream = fn;
    lexer->current  = NULL;
    lexer->next     = NULL;
    lexer->previous = NULL;
    (void) lexer_next(lexer);

    return;
}

ClaspToken *lexer_next(ClaspLexer *lexer) {
    if (lexer->current == NULL) {  // initialize
        lexer->current = lexer_scan(lexer);
        lexer->next    = lexer_scan(lexer);
    } else {
        lexer->previous = lexer->current;
        lexer->current  = lexer->next;
        lexer->next     = lexer_scan(lexer);
    }
}

static bool is_identifier(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c == '_');
}

ClaspToken *lexer_scan(ClaspLexer *lexer) {
    char current = lexer->stream();
    if (is_identifier(current)) {
        char *identifier = malloc(128);
        unsigned int length = 1;
        identifier[0] = current;

        while ((is_identifier(current) || isdigit(current)) && length < 128) {
            current = lexer->stream();
            identifier[length++] = current;
        }
        char *final = malloc(length + 1);
        memcpy(final, current, length);
        final[length] = '\0';

        return ; // TODO: return an ID token
    }
}