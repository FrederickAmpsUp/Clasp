/**
 * Clasp visitor function declaration
 * Authored 1/3/2024-present
 * 
 * This program is part of the Clasp Header Libraries
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

#ifndef VISITOR_H
#define VISITOR_H

#include <clasp/ast.h>

/**
 * Visitor functions for different AST node types.
 * These are automatically called by the visit() function in clasp/ast.h
*/
void *visit_binop(ClaspASTNode *binop, void *args);
void *visit_unop(ClaspASTNode *unop, void *args);
void *visit_postfix(ClaspASTNode *postfix, void *args);
void *visit_lit_num(ClaspASTNode *lit, void *args);
void *visit_var_ref(ClaspASTNode *var, void *args);
void *visit_fn_call(ClaspASTNode *fn, void *args);

void *visit_return_stmt(ClaspASTNode *retval, void *args);
void *visit_expr_stmt(ClaspASTNode *expr, void *args);
void *visit_block_stmt(ClaspASTNode *block, void *args);
void *visit_var_decl(ClaspASTNode *var, void *args);
void *visit_fn_decl(ClaspASTNode *fn, void *args);

void *visit_if(ClaspASTNode *stmt, void *args);
void *visit_while(ClaspASTNode *stmt, void *args);

void *visit_single_type(ClaspASTNode *type, void *args);

ClaspASTVisitor self_visitor = {
    [AST_EXPR_BINOP     ] = &visit_binop,
    [AST_EXPR_UNOP      ] = &visit_unop,
    [AST_EXPR_POSTFIX   ] = &visit_postfix,
    [AST_EXPR_LIT_NUMBER] = &visit_lit_num,
    [AST_EXPR_VAR_REF   ] = &visit_var_ref,
    [AST_EXPR_FN_CALL   ] = &visit_fn_call,

    [AST_RETURN_STMT    ] = &visit_return_stmt,
    [AST_EXPR_STMT      ] = &visit_expr_stmt,
    [AST_BLOCK_STMT     ] = &visit_block_stmt,
    [AST_VAR_DECL_STMT  ] = &visit_var_decl,
    [AST_LET_DECL_STMT  ] = &visit_var_decl,
    [AST_CONST_DECL_STMT] = &visit_var_decl,

    [AST_FN_DECL_STMT   ] = &visit_fn_decl,

    [AST_IF_STMT        ] = &visit_if,
    [AST_WHILE_STMT     ] = &visit_while,

    [AST_TYPE_SINGLE    ] = &visit_single_type,
};

#endif // VISITOR_H