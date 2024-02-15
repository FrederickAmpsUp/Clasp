/**
 * Clasp AST printer implementing the target API.
 * Authored 1/10/2024
 * 
 * This program is part of the Clasp Source Libraries
 * 
 * Copyright (c) 2024, Frederick Ziola
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

#include <clasp/clasp.h>
#include <clasp/visitor.h>

void *visit_binop(ClaspASTNode *binop, void *args) {
    printf("(binop: left=");
    visit(binop->data.binop.left, args, self_visitor);
    printf(" op=%s right=", binop->data.binop.op->data);
    visit(binop->data.binop.right, args, self_visitor);
    printf(")");
    return NULL;
}
void *visit_unop(ClaspASTNode *unop, void *args) {
    printf("(unop: op=%s right=", unop->data.unop.op->data);
    visit(unop->data.unop.right, args, self_visitor);
    printf(")");
    return NULL;
}

void *visit_postfix(ClaspASTNode *post, void *args) {
    printf("(postfix: left=");
    visit(post->data.postfix.left, args, self_visitor);
    printf(" op=%s)", post->data.postfix.op->data);
    return NULL;
}
void *visit_lit_num(ClaspASTNode *lit, void *args) {
    printf("(num_literal: val=%s)", lit->data.lit_num.value->data);
    return NULL;
}

void *visit_var_ref(ClaspASTNode *var, void *args) {
    printf("(var_ref: name=%s)", var->data.var_ref.varname->data);
    return NULL;
}

void *visit_fn_call(ClaspASTNode *fn, void *args) {
    printf("(fn_call: ref=");
    visit(fn->data.fn_call.referencer, args, self_visitor);
    printf(" args=[  ");

    for (int i = 0; i < cvector_size(fn->data.fn_call.args); ++i) {
        printf("\b\b");
        visit(fn->data.fn_call.args[i], args, self_visitor);
        printf(",   ");
    }
    printf("\b\b\b\b])");
}

void *visit_return_stmt(ClaspASTNode *ast, void *args) {
    printf("(returnStmt: ");
    visit(ast->data.return_stmt.retval, args, self_visitor);
    printf(")\n");
    return NULL;
}

void *visit_expr_stmt(ClaspASTNode *ast, void *args) {
    printf("(exprStmt: ");
    visit(ast->data.expr_stmt.expr, args, self_visitor);
    printf(")\n");
    return NULL;
}

void *visit_block_stmt(ClaspASTNode *ast, void *args) {
    printf("(blockStmt:\n");
    for (int i = 0; i < cvector_size(ast->data.block_stmt.body); ++i) {
        visit(ast->data.block_stmt.body[i], args, self_visitor);
    }
    printf(")\n");
    return NULL;
}

void *visit_var_decl(ClaspASTNode *ast, void *args) {
    switch (ast->type) {
        case AST_VAR_DECL_STMT:   printf("(varDecl:");   break;
        case AST_LET_DECL_STMT:   printf("(letDecl:");   break;
        case AST_CONST_DECL_STMT: printf("(constDecl:"); break;
    }
    printf(" name=\"%s\"", ast->data.var_decl_stmt.name->data);
    if (ast->data.var_decl_stmt.type) {
        printf(" type=");
        visit(ast->data.var_decl_stmt.type, args, self_visitor);
    }

    if (ast->data.var_decl_stmt.initializer) {
        printf(" initializer=");
        visit(ast->data.var_decl_stmt.initializer, args, self_visitor);
    }
    printf(")\n");
}
void *visit_let_decl(ClaspASTNode *ast, void *args) {
    return visit_var_decl(ast, args);
}
void *visit_const_decl(ClaspASTNode *ast, void *args) {
    return visit_var_decl(ast, args);
}

void *visit_fn_decl(ClaspASTNode *ast, void *args) {
    printf("fnDecl: name=\"%s\" ret=", ast->data.fn_decl_stmt.name->data);
    visit(ast->data.fn_decl_stmt.ret_type, args, self_visitor);
    printf(" args=[  ");
    for (int i = 0; i < cvector_size(ast->data.fn_decl_stmt.args); ++i) {
        struct ClaspArg *arg = ast->data.fn_decl_stmt.args[i];
        printf("\b\b(%s ", arg->name->data);
        visit(arg->type, args, self_visitor);
        printf("),   ");
    }
    printf("\b\b\b\b] body=");
    visit(ast->data.fn_decl_stmt.body, args, self_visitor);
    printf(")\n");
}

void *visit_if(ClaspASTNode *ast, void *args) {
    printf("(ifStmt: cond=");
    visit(ast->data.cond_stmt.cond, args, self_visitor);
    printf(" body=");
    visit(ast->data.cond_stmt.body, args, self_visitor);
    printf(")\n");
}

void *visit_while(ClaspASTNode *ast, void *args) {
    printf("(whileStmt: cond=");
    visit(ast->data.cond_stmt.cond, args, self_visitor);
    printf(" body=");
    visit(ast->data.cond_stmt.body, args, self_visitor);
    printf(")\n");
}

void *visit_single_type(ClaspASTNode *ast, void *args) {
    printf("[single name=\"%s\"]", ast->data.single.name->data);
}

void target_run(ClaspASTNode *ast, void *args) {
    (void) visit(ast, args, self_visitor);
}