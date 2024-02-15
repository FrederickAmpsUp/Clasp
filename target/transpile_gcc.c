/**
 * Clasp Template AST visitor target implementing the target API
 * Authored 1/11/2024
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
    int tabs = *(int*)args;
    visit(binop->data.binop.left, &tabs, self_visitor);
    ClaspToken *op = binop->data.binop.op;
    printf(" %s ", op->data);
    visit(binop->data.binop.right, &tabs, self_visitor);
    return NULL;
}

void *visit_unop(ClaspASTNode *unop, void *args) {
    int tabs = *(int*)args;
    ClaspToken *op = unop->data.unop.op;
    printf("%s", op->data);
    visit(unop->data.unop.right, &tabs, self_visitor);
    return NULL;
}

void *visit_postfix(ClaspASTNode *postfix, void *args) {
    int tabs = *(int*)args;
    visit(postfix->data.postfix.left, &tabs, self_visitor);
    ClaspToken *op = postfix->data.postfix.op;
    printf("%s", op->data);
    return NULL;
}

void *visit_lit_num(ClaspASTNode *lit, void *args) {
    ClaspToken *num = lit->data.lit_num.value;
    printf("%s", num->data);
    return NULL;
}

void *visit_var_ref(ClaspASTNode *var, void *args) {
    ClaspToken *ref = var->data.var_ref.varname;
    printf("%s", ref->data);
    return NULL;
}

void *visit_fn_call(ClaspASTNode *fn, void *args) {
    int tabs = *(int*)args;
    visit(fn->data.fn_call.referencer, &tabs, self_visitor);
    printf("(");
    for (size_t i = 0; i < cvector_size(fn->data.fn_call.args); i++) {
        ClaspASTNode *arg = fn->data.fn_call.args[i];
        visit(arg, &tabs, self_visitor);
        if (i == cvector_size(fn->data.fn_call.args)-1) break;
        printf(", ");
    }
    printf(")");
    return NULL;
}

#define TABS(n) do { for (int __I = 0; __I < abs(n)-2; __I++) putchar('\t'); } while (0)

void *visit_expr_stmt(ClaspASTNode *expr, void *args) {
    int tabs = *(int*)args;
    TABS(tabs);

    visit(expr->data.expr_stmt.expr, &tabs, self_visitor);
    printf(";\n");
    return NULL;
}

void *visit_block_stmt(ClaspASTNode *block, void *args) {
    int tabs = *(int*)args;
    if (tabs > 0) TABS(abs(tabs));
    tabs = abs(*(int*)args) + 1;

    if (*(int*)args != -1) printf("{\n");
    for (int i = 0; i < cvector_size(block->data.block_stmt.body); i++) {
        ClaspASTNode *stmt = block->data.block_stmt.body[i];
        visit(stmt, &tabs, self_visitor);
    }
    TABS(abs(*(int*)args));
    if (*(int*)args != -1) printf("}\n");
    return NULL;
}

void *visit_var_decl(ClaspASTNode *var, void *args) {
    int tabs = *(int*)args;
    TABS(tabs);

    switch (var->type) {
        case AST_LET_DECL_STMT:
        case AST_CONST_DECL_STMT:
            printf("const ");
            break;
    }
    if (var->data.var_decl_stmt.type == NULL) fprintf(stderr, "Error: no specified type for variable '%s'.\n", var->data.var_decl_stmt.name->data);
    visit(var->data.var_decl_stmt.type, &tabs, self_visitor);
    printf(" %s", var->data.var_decl_stmt.name->data);
    if (var->data.var_decl_stmt.initializer != NULL) {
        printf(" = ");
        visit(var->data.var_decl_stmt.initializer, &tabs, self_visitor);
    }
    printf(";\n");
    return NULL;
}

void *visit_fn_decl(ClaspASTNode *fn, void *args) {
    int tabs = *(int*)args;
    TABS(tabs);
    visit(fn->data.fn_decl_stmt.ret_type, &tabs, self_visitor);
    printf(" %s(", fn->data.fn_decl_stmt.name->data);
    for (size_t i = 0; i < cvector_size(fn->data.fn_decl_stmt.args); i++) {
        struct ClaspArg *arg = fn->data.fn_decl_stmt.args[i];
        visit(arg->type, &tabs, self_visitor);
        printf(" %s", arg->name->data);
        if (i == cvector_size(fn->data.fn_decl_stmt.args)-1) break;
        printf(", ");
    }
    printf(") ");
    tabs = -tabs;
    visit(fn->data.fn_decl_stmt.body, &tabs, self_visitor);
    return NULL;
}

void *visit_if(ClaspASTNode *stmt, void *args) {
    int tabs = *(int*)args;
    TABS(tabs);

    printf("if (");
    visit(stmt->data.cond_stmt.cond, &tabs, self_visitor);
    printf(") ");
    tabs = -tabs;
    visit(stmt->data.cond_stmt.body, &tabs, self_visitor);
    return NULL;
}

void *visit_while(ClaspASTNode *stmt, void *args) {
    int tabs = *(int*)args;
    TABS(tabs);

    printf("while (");
    visit(stmt->data.cond_stmt.cond, &tabs, self_visitor);
    printf(") ");
    tabs = -tabs;
    visit(stmt->data.cond_stmt.body, &tabs, self_visitor);
    return NULL;
}

void *visit_single_type(ClaspASTNode *type, void *args) {
    printf("%s", type->data.single.name->data);
    return NULL;
}

void target_run(ClaspASTNode *ast, void *args) {
    int tabs = -1;
    visit(ast, &tabs, self_visitor);
}