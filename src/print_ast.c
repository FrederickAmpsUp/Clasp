/**
 * Clasp Abstract Syntax Tree Printer Implementation
 * Authored 12/15/2023-present
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

void claspPrintAST(ClaspASTNode *ast) {
    visit(ast, clasp_ast_printer);
    return NULL;
}

ClaspASTVisitor clasp_ast_printer = {
    [AST_BINOP     ] = &claspPrintBinop,
    [AST_UNOP      ] = &claspPrintUnop,
    [AST_LIT_NUMBER] = &claspPrintNumLiteral,
};