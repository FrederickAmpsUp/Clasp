#ifndef PARSER_H
#define PARSER_H

#include <clasp/lexer.h>
#include <stdio.h>

typedef void (*ClaspOutputFn)(ClaspToken *token, FILE *out);

typedef ClaspOutputFn ClaspVisitorTable[1];

typedef enum {
    VISITOR_BINOP,
} ClaspVisitorTypes;

void clasp_visit_binop(ClaspToken *token, FILE *out);

const ClaspVisitorTable claspDefaultVisitors = {
    [VISITOR_BINOP] = &clasp_visit_binop,
};

typedef struct {
    ClaspLexer *lexer;
    FILE *out;
    const ClaspVisitorTable *visitors;
} ClaspParser;

ClaspParser *new_parser(ClaspLexer *lexer, FILE *out);
ClaspParser *new_parser_custom(ClaspLexer *lexer, FILE *out, ClaspVisitorTable *visitors);

void parser_compile(ClaspParser *parser);

/**
 * Order of operations:
 * no
*/

void parser_expression(ClaspParser *parser);  // Expression statement
void parser_term(ClaspParser *parser);        // Add/sub

void parser_primary(ClaspParser *parser);     // Primary (parenthesis, numbers, ... stuff)


#endif // PARSER_H