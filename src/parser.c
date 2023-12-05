#include <clasp/parser.h>
#include <stdlib.h>

ClaspParser *new_parser(ClaspLexer *l, FILE *out) {
    ClaspParser *ret = malloc(sizeof(ClaspParser));
    ret->lexer = l;
    ret->out = out;
    ret->visitors = &claspDefaultVisitors;

    return ret;
}
ClaspParser *new_parser_custom(ClaspLexer *l, FILE *out, ClaspVisitorTable *v) {
    ClaspParser *ret = malloc(sizeof(ClaspParser));
    ret->lexer = l;
    ret->out = out;
    ret->visitors = v;

    return ret;
}

void parser_compile(ClaspParser *p) {
    parser_expression(p); // TODO: declstatements
}
void parser_expression(ClaspParser *p) {
    parser_term(p);
}