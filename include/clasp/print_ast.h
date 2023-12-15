#ifndef PRINT_AST_H
#define PRINT_AST_H

#include <clasp/ast.h>

void *claspPrintBinop(ClaspASTNode *binop);
void *claspPrintUnop(ClaspASTNode *unop);
void *claspPrintNumLiteral(ClaspASTNode *lit);

void *claspPrintAST(ClaspASTNode *ast);

extern ClaspASTVisitor clasp_ast_printer;

#endif // PRINT_AST_H