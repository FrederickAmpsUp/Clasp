/**
 * Clasp language parser declaration
 * Authored 12/2023-present
 * 
 * This program is part of the Clasp Header Libraries
 * 
 * Copyright (c) 2023, Frederick Ziola
 *                      frederick.ziola@gmail.com
 * 
 * SPDX-License-Identifier: GPL-3.0
 * 
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef PARSER_H
#define PARSER_H

#include <clasp/lexer.h>
#include <stdio.h>
#include <clasp/sc_array.h>
#include <clasp/ast.h>

/**
 * State of a parser. Stores the lexer used.
*/
typedef struct {
    ClaspLexer *lexer;
} ClaspParser;

/**
 * Initialize a new parser.
 * @param parser The parser to initialize
 * @param lexer The lexer state for the parser to use
*/
void new_parser(ClaspParser *parser, ClaspLexer *lexer);

/**
 * Parse the input into an AST.
 * @param parser The parser state to parse from.
 * @return The AST from the parser's input.
*/
ClaspASTNode *parser_compile(ClaspParser *parser);

/**
 * Order of operations:
 * see spec/precedence.md
*/

/**
 * Parse an expression. This should only be called internally, except special cases.
*/
ClaspASTNode *parser_expression(ClaspParser *parser);  // Expression statement
/**
 * Parse a term. This should only be called internally, except special cases.
*/
ClaspASTNode *parser_term(ClaspParser *parser);        // Add/sub
/**
 * Parse a factor. This should only be called internally, except special cases.
*/
ClaspASTNode *parser_factor(ClaspParser *parser);      // Mul/div
/**
 * Parse an exponent. This should only be called internally, except special cases.
*/
ClaspASTNode *parser_exponent(ClaspParser *parser);    // Exponentiation
/**
 * Parse a unary op. This should only be called internally, except special cases.
*/
ClaspASTNode *parser_unary(ClaspParser *parser);       // Unary operators
/**
 * Parse a primary. This should only be called internally, except special cases.
*/
ClaspASTNode *parser_primary(ClaspParser *parser);     // Primary (parenthesis, numbers, ... stuff)


#endif // PARSER_H