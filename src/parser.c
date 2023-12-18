/**
 * Clasp language Parser Implementation
 * Authored 12/2023-present
 * 
 * This program is part of the Clasp Source Libraries
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

#include <clasp/parser.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdarg.h>
#include <clasp/ast.h>
#include <clasp/err.h>
#include <cvector/cvector.h>

void new_parser(ClaspParser *p, ClaspLexer *l) {
    p->lexer = l;
}
#define consume(p, t, ...) consume_impl(p, t, (sizeof((int[]){__VA_ARGS__})/sizeof(int)), __VA_ARGS__)
static bool consume_impl(ClaspParser *p, ClaspToken **t, int n, ...) {
    va_list ptr;
    va_start(ptr, n);

    for (int i = 0; i < n; ++i) {
        if (lexer_has(p->lexer, va_arg(ptr, ClaspTokenType))) {
            if (t) *t = lexer_next(p->lexer);
            else (void) lexer_next(p->lexer);
            va_end(ptr);
            return true;
        }
    }
    va_end(ptr);
    if (t) *t = NULL;
    return false;
}

ClaspASTNode *parser_compile(ClaspParser *p) {
    cvector(ClaspASTNode *) block;
    while (!consume(p, NULL, TOKEN_EOF)) {
        cvector_push_back(block, parser_stmt(p));
    }
    return block_stmt(block);
}

ClaspASTNode *parser_stmt(ClaspParser *p) {
    // TODO: declstmts

    ClaspASTNode *expr = parser_expression(p);
    if (!consume(p, NULL, TOKEN_SEMICOLON)) {
        general_err("Expected semicolon after expression statement.\n");
    } return expr_stmt(expr);
}

ClaspASTNode *parser_expression(ClaspParser *p) {
    return parser_term(p);
}
ClaspASTNode *parser_term(ClaspParser *p) {
    ClaspASTNode *left = parser_factor(p);  // Left operand

    ClaspToken *op;
    while (consume(p, &op, TOKEN_PLUS, TOKEN_MINUS)) {
        ClaspASTNode *right = parser_factor(p);
        left = binop(left, right, op);
    }
    return left;
}
ClaspASTNode *parser_factor(ClaspParser *p) {
    ClaspASTNode *left = parser_exponent(p); // Left operand

    ClaspToken *op;
    while (consume(p, &op, TOKEN_ASTERIX, TOKEN_SLASH, TOKEN_PERC)) {
        ClaspASTNode *right = parser_exponent(p); // Right operand
        left = binop(left, right, op);
    }
    return left;
}
ClaspASTNode *parser_exponent(ClaspParser *p) {
    ClaspASTNode *left = parser_unary(p); // left operand

    ClaspToken *op;
    while (consume(p, &op, TOKEN_CARAT)) {
        ClaspASTNode *right = parser_exponent(p); // right operand, right-associativity
        left = binop(left, right, op);
    }
    return left;
}
ClaspASTNode *parser_unary(ClaspParser *p) {
    ClaspToken *op;
    if (consume(p, &op, TOKEN_PLUS, TOKEN_MINUS)) {
        ClaspASTNode *right = parser_unary(p); // right operand
        return unop(right, op);
    }
    return parser_primary(p);
}
ClaspASTNode *parser_primary(ClaspParser *p) {
    ClaspToken *num;
    if (consume(p, &num, TOKEN_NUMBER)) { // Numeric literals
        return lit_num(num);
    }


}