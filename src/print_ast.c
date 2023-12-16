#include <clasp/print_ast.h>
#include <stdio.h>

void *claspPrintBinop(ClaspASTNode *binop) {
    printf("(binop: left=");
    visit(binop->data.binop.left, clasp_ast_printer);
    printf(" op=%s right=", binop->data.binop.op->data);
    visit(binop->data.binop.right, clasp_ast_printer);
    printf(")");
    return NULL;
}
void *claspPrintUnop(ClaspASTNode *unop) {
    printf("(unnop: op=%s right=", unop->data.unop.op->data);
    visit(unop->data.unop.right, clasp_ast_printer);
    printf(")");
    return NULL;
}
void *claspPrintNumLiteral(ClaspASTNode *lit) {
    printf("(num_literal: val=%s)", lit->data.lit_num.value->data);
    return NULL;
}

void *claspPrintAST(ClaspASTNode *ast) {
    visit(ast, clasp_ast_printer);
    return NULL;
}

ClaspASTVisitor clasp_ast_printer = {
    [AST_BINOP     ] = &claspPrintBinop,
    [AST_UNOP      ] = &claspPrintUnop,
    [AST_LIT_NUMBER] = &claspPrintNumLiteral,
};