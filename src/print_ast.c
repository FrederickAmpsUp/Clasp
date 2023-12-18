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
#include <cvector/cvector.h>

void *_printBinop(ClaspASTNode *binop) {
    printf("(binop: left=");
    visit(binop->data.binop.left, clasp_ast_printer);
    printf(" op=%s right=", binop->data.binop.op->data);
    visit(binop->data.binop.right, clasp_ast_printer);
    printf(")");
    return NULL;
}
void *_printUnop(ClaspASTNode *unop) {
    printf("(unop: op=%s right=", unop->data.unop.op->data);
    visit(unop->data.unop.right, clasp_ast_printer);
    printf(")");
    return NULL;
}

void *_printPostfix(ClaspASTNode *post) {
    printf("(postfix: left=");
    visit(post->data.postfix.left, clasp_ast_printer);
    printf(" op=%s)", post->data.postfix.op->data);
    return NULL;
}
void *_printNumLiteral(ClaspASTNode *lit) {
    printf("(num_literal: val=%s)", lit->data.lit_num.value->data);
    return NULL;
}

void *_printVarRef(ClaspASTNode *var) {
    printf("(var_ref: name=%s)", var->data.var_ref.varname->data);
    return NULL;
}

void *_printFnCall(ClaspASTNode *fn) {
    printf("(fn_call: ref=");
    visit(fn->data.fn_call.referencer, clasp_ast_printer);
    printf(" args=[  ");

    for (int i = 0; i < cvector_size(fn->data.fn_call.args); ++i) {
        printf("\b\b");
        visit(fn->data.fn_call.args[i], clasp_ast_printer);
        printf(",   ");
    }
    printf("\b\b\b\b])");
}

void *_printExprStmt(ClaspASTNode *ast) {
    printf("(exprStmt: ");
    visit(ast->data.expr_stmt.expr, clasp_ast_printer);
    printf(")\n");
    return NULL;
}

void *_printBlockStmt(ClaspASTNode *ast) {
    printf("(blockStmt:\n");
    for (int i = 0; i < cvector_size(ast->data.block_stmt.body); ++i) {
        printf("\t");
        visit(ast->data.block_stmt.body[i], clasp_ast_printer);
    }
    printf(")\n");
    return NULL;
}

void claspPrintAST(ClaspASTNode *ast) {
    visit(ast, clasp_ast_printer);
}

ClaspASTVisitor clasp_ast_printer = {
    [AST_EXPR_BINOP     ] = &_printBinop,
    [AST_EXPR_UNOP      ] = &_printUnop,
    [AST_EXPR_POSTFIX   ] = &_printPostfix,
    [AST_EXPR_LIT_NUMBER] = &_printNumLiteral,
    [AST_EXPR_VAR_REF   ] = &_printVarRef,
    [AST_EXPR_FN_CALL   ] = &_printFnCall,
    [AST_EXPR_STMT      ] = &_printExprStmt,
    [AST_BLOCK_STMT     ] = &_printBlockStmt,
};