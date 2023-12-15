#include <clasp/parser.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdarg.h>
#include <clasp/ast.h>
void new_parser(ClaspParser *p, ClaspLexer *l, FILE *out) {
    p->lexer = l;
    p->out = out;
}
void new_parser_custom(ClaspParser *p, ClaspLexer *l, FILE *out) {
    p->lexer = l;
    p->out = out;
}
#define consume(p, ...)  consume_impl(p, (sizeof((int[]){__VA_ARGS__})/sizeof(int)), __VA_ARGS__)
static bool consume_impl(ClaspParser *p, int n, ...) {
    va_list ptr;
    va_start(ptr, n);

    for (int i = 0; i < n; ++i) {
        if (lexer_has(p->lexer, va_arg(ptr, ClaspTokenType))) {
            (void) lexer_next(p->lexer);
            va_end(ptr);
            return true;
        }
    }
    va_end(ptr);
    return false;
}

ClaspASTNode *parser_compile(ClaspParser *p) {
    return parser_expression(p); // TODO: declstatements
}
ClaspASTNode *parser_expression(ClaspParser *p) {
    return parser_term(p);
}
ClaspASTNode *parser_term(ClaspParser *p) {
    ClaspASTNode *left = parser_factor(p);  // Left operand

    while (consume(p, TOKEN_PLUS, TOKEN_MINUS)) {
        ClaspASTNode *right = parser_factor(p);
        left = binop(left, right, p->lexer->previous);
    }
    return left;
}
ClaspASTNode *parser_factor(ClaspParser *p) {
    ClaspASTNode *left = parser_exponent(p); // Left operand

    while (consume(p, TOKEN_ASTERIX, TOKEN_SLASH, TOKEN_PERC)) {
        ClaspASTNode *right = parser_exponent(p); // Right operand
        left = binop(left, right, p->lexer->previous);
    }
    return left;
}
ClaspASTNode *parser_exponent(ClaspParser *p) {
    ClaspASTNode *left = parser_unary(p); // left operand

    while (consume(p, TOKEN_CARAT)) {
        ClaspASTNode *right = parser_exponent(p); // right operand, right-associativity
        left = binop(left, right, p->lexer->previous);
    }
    return left;
}
ClaspASTNode *parser_unary(ClaspParser *p) {
    if (consume(p, TOKEN_PLUS, TOKEN_MINUS)) {
        ClaspToken *op = p->lexer->previous;
        ClaspASTNode *right = parser_unary(p); // right operand
        return unop(right, op);
    }
    return parser_primary(p);
}
ClaspASTNode *parser_primary(ClaspParser *p) {
    if (consume(p, TOKEN_NUMBER)) { // Numeric literals
        return lit_num(p->lexer->previous);
    }
}

void clasp_visit_binop(ClaspToken *t, FILE *o) {} // TODO: this