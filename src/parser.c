#include <clasp/parser.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdlib.h>

/**
 * TODO: type system (how??)
*/

static const ClaspVisitorTable claspDefaultVisitors = {
    [VISITOR_BINOP] = &clasp_visit_binop,
};

void new_parser(ClaspParser *p, ClaspLexer *l, FILE *out) {
    p->lexer = l;
    p->out = out;
    p->visitors = &claspDefaultVisitors;
}
void new_parser_custom(ClaspParser *p, ClaspLexer *l, FILE *out, ClaspVisitorTable *v) {
    p->lexer = l;
    p->out = out;
    p->visitors = v;
}

static bool consume(ClaspParser *p, ClaspTokenType t) {
    if (lexer_has(p->lexer, t)) {
        (void) lexer_next(p->lexer);
        return true;
    } return false;
}

void parser_compile(ClaspParser *p) {
    parser_expression(p); // TODO: declstatements
}
void parser_expression(ClaspParser *p) {
    parser_term(p);
}
void parser_term(ClaspParser *p) {
    parser_factor(p);  // Left operand

    while (lexer_has(p->lexer, TOKEN_PLUS) || lexer_has(p->lexer, TOKEN_MINUS)) {
            bool add;
            if (consume(p, TOKEN_PLUS)) {
                add = true;
            } else if (consume(p, TOKEN_MINUS)) {
                add = false;
            }

            parser_factor(p); // Right operand
            if (add) {
                printf("add\n");
            } else { // TODO: emit opcodes
                printf("sub\n");
            }
    }
}
void parser_factor(ClaspParser *p) {
    parser_primary(p); // TODO: factor stuff
}
void parser_primary(ClaspParser *p) {
    if (lexer_has(p->lexer, TOKEN_NUMBER)) { // Numeric literals
        ClaspToken *num = lexer_next(p->lexer);
        printf("const %s\n", num->data);
        return;
    }
}

void clasp_visit_binop(ClaspToken *t, FILE *o) {} // TODO: this