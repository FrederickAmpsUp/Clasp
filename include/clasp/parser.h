#ifndef PARSER_H
#define PARSER_H

#include <clasp/lexer.h>
#include <stdio.h>
#include <clasp/sc_array.h>

typedef void (*ClaspOutputFn)(ClaspToken *token, FILE *out);

typedef ClaspOutputFn ClaspVisitorTable[1];

typedef enum {
    VISITOR_BINOP,
} ClaspVisitorTypes;


void clasp_visit_binop(ClaspToken *token, FILE *out);

typedef struct {
    ClaspLexer *lexer;
    FILE *out;
    const ClaspVisitorTable *visitors;
} ClaspParser;

void new_parser(ClaspParser *parser, ClaspLexer *lexer, FILE *out);
void new_parser_custom(ClaspParser *parser, ClaspLexer *lexer, FILE *out, ClaspVisitorTable *visitors);

void parser_compile(ClaspParser *parser);

/**
 * Order of operations:
 * see spec/precedence.md
*/

void parser_expression(ClaspParser *parser);  // Expression statement
void parser_term(ClaspParser *parser);        // Add/sub
void parser_factor(ClaspParser *parser);      // Mul/div
void parser_exponent(ClaspParser *parser);    // Exponentiation
void parser_unary(ClaspParser *parser);       // Unary operators

void parser_primary(ClaspParser *parser);     // Primary (parenthesis, numbers, ... stuff)


#endif // PARSER_H