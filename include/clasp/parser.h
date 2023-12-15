#ifndef PARSER_H
#define PARSER_H

#include <clasp/lexer.h>
#include <stdio.h>
#include <clasp/sc_array.h>
#include <clasp/ast.h>

typedef struct {
    ClaspLexer *lexer;
    FILE *out;
} ClaspParser;

void new_parser(ClaspParser *parser, ClaspLexer *lexer, FILE *out);

ClaspASTNode *parser_compile(ClaspParser *parser);

/**
 * Order of operations:
 * see spec/precedence.md
*/

ClaspASTNode *parser_expression(ClaspParser *parser);  // Expression statement
ClaspASTNode *parser_term(ClaspParser *parser);        // Add/sub
ClaspASTNode *parser_factor(ClaspParser *parser);      // Mul/div
ClaspASTNode *parser_exponent(ClaspParser *parser);    // Exponentiation
ClaspASTNode *parser_unary(ClaspParser *parser);       // Unary operators

ClaspASTNode *parser_primary(ClaspParser *parser);     // Primary (parenthesis, numbers, ... stuff)


#endif // PARSER_H