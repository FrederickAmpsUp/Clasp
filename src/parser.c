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
        } else {
            fprintf(stderr, "impossible error, please report this message: \n\n\"unexpected token with type %s whilst parsing term\"\n", tktyp_str(lexer_next(p->lexer)->type));
            exit(2);
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
    parser_exponent(p); // Left operand

    while (lexer_has(p->lexer, TOKEN_ASTERIX)
        || lexer_has(p->lexer, TOKEN_SLASH)
        || lexer_has(p->lexer, TOKEN_PERC)) {
        int op;
        if (consume(p, TOKEN_ASTERIX)) {
            op = 0; // mul
        } else if (consume(p, TOKEN_SLASH)) {
            op = 1; // div
        } else if (consume(p, TOKEN_PERC)) {
            op = 2; // mod
        } else {
            fprintf(stderr, "impossible error, please report this message: \n\n\"unexpected token with type %s whilst parsing factor\"\n", tktyp_str(lexer_next(p->lexer)->type));
            exit(2);
        }

        parser_exponent(p); // Right operand
        if (op == 0) {  // multiplication
            printf("mul\n");
        } else if (op == 1) {  // division 
            printf("div\n");
        } else if (op == 2) {  // modulo
            printf("mod\n");
        }
    }
}
void parser_exponent(ClaspParser *p) {
    parser_unary(p);

    while (lexer_has(p->lexer, TOKEN_CARAT)) {
        int op;
        if (consume(p, TOKEN_CARAT)) {
            op = 0; // exponent
        } else {
            fprintf(stderr, "impossible error, please report this message: \n\n\"unexpected token with type %s whilst parsing exponent\"\n", tktyp_str(lexer_next(p->lexer)->type));
            exit(2);
        }

        parser_exponent(p);
        if (op == 0) {  // exponentiation
            printf("pow\n");
        }
    }
}
void parser_unary(ClaspParser *p) {
    if (lexer_has(p->lexer, TOKEN_PLUS)
     || lexer_has(p->lexer, TOKEN_MINUS)) {
        int op;
        if (consume(p, TOKEN_PLUS)) {
            op = 0; // Unary nothing (+5 is just 5)
        } else if (consume(p, TOKEN_MINUS)) {
            op = 1; // Unary negation
        } else {
            fprintf(stderr, "impossible error, please report this message: \n\n\"unexpected token with type %s whilst parsing unary\"\n", tktyp_str(lexer_next(p->lexer)->type));
            exit(2);
        }

        parser_unary(p); // right operand

        if (op == 0) {
            printf("iden\n");
        } else if (op == 1) {
            printf("neg\n");
        }
     }
    parser_primary(p);
}
void parser_primary(ClaspParser *p) {
    if (lexer_has(p->lexer, TOKEN_NUMBER)) { // Numeric literals
        ClaspToken *num = lexer_next(p->lexer);
        printf("const %s\n", num->data);
        return;
    }
}

void clasp_visit_binop(ClaspToken *t, FILE *o) {} // TODO: this