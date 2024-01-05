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
void *visitBinop(ClaspASTNode *binop);
void *visitUnop(ClaspASTNode *unop);
void *visitPostfix(ClaspASTNode *postfix);
void *visitNumLiteral(ClaspASTNode *lit);
void *visitVarRef(ClaspASTNode *var);
void *visitFnCall(ClaspASTNode *fn);

void *visitExprStmt(ClaspASTNode *expr);
void *visitBlockStmt(ClaspASTNode *block);
void *visitVarDecl(ClaspASTNode *var);
void *visitFnDecl(ClaspASTNode *fn);

void *visitIfStmt(ClaspASTNode *stmt);
void *visitWhileStmt(ClaspASTNode *stmt);

void *visitSingleType(ClaspASTNode *type);

ClaspASTVisitor clasp_ast_printer = {
    [AST_EXPR_BINOP     ] = &visitBinop,
    [AST_EXPR_UNOP      ] = &visitUnop,
    [AST_EXPR_POSTFIX   ] = &visitPostfix,
    [AST_EXPR_LIT_NUMBER] = &visitNumLiteral,
    [AST_EXPR_VAR_REF   ] = &visitVarRef,
    [AST_EXPR_FN_CALL   ] = &visitFnCall,

    [AST_EXPR_STMT      ] = &visitExprStmt,
    [AST_BLOCK_STMT     ] = &visitBlockStmt,
    [AST_VAR_DECL_STMT  ] = &visitVarDecl,
    [AST_LET_DECL_STMT  ] = &visitVarDecl,
    [AST_CONST_DECL_STMT] = &visitVarDecl,

    [AST_FN_DECL_STMT   ] = &visitFnDecl,

    [AST_IF_STMT        ] = &visitIfStmt,
    [AST_WHILE_STMT     ] = &visitWhileStmt,

    [AST_TYPE_SINGLE    ] = &visitSingleType,
};

#endif // VISITOR_H