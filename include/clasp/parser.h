#ifndef PARSER_H
#define PARSER_H

#include <clasp/lexer.h>
#include <stdio.h>

typedef struct {
    ClaspLexer *lexer;

    FILE *out;
} ClaspParser;

ClaspParser *new_parser(ClaspLexer *lexer, FILE *out);

#endif // PARSER_H