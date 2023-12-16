/**
 * Clasp Abstract Syntax Tree declaration
 * Authored 12/15/2023-present
 * 
 * This program is part of the Clasp Header Libraries
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

#ifndef AST_H
#define AST_H

#include <clasp/lexer.h>

typedef enum {
    AST_BINOP,
    AST_UNOP,
    AST_LIT_NUMBER,

    CLASP_NUM_VISITORS
} ClaspASTNodeType;

typedef struct ClaspASTNode ClaspASTNode;

union ASTNodeData {
    struct {
        ClaspASTNode *left;
        ClaspASTNode *right;
        ClaspToken *op;
    } binop;
    struct {
        ClaspASTNode *right;
        ClaspToken *op;
    } unop;
    struct {
        ClaspToken *value;
    } lit_num;
};
typedef struct ClaspASTNode {
    ClaspASTNodeType type;
    union ASTNodeData data;
} ClaspASTNode;

ClaspASTNode *new_node(ClaspASTNodeType type, union ASTNodeData *data);
ClaspASTNode *binop(ClaspASTNode *left, ClaspASTNode *right, ClaspToken *op);
ClaspASTNode *unop(ClaspASTNode *right, ClaspToken *op);
ClaspASTNode *lit_num(ClaspToken *num);

typedef void *(*ClaspVisitorFn) (ClaspASTNode *node);
typedef ClaspVisitorFn ClaspASTVisitor[CLASP_NUM_VISITORS];

void *visit(ClaspASTNode *node, ClaspASTVisitor visitor);

#endif // AST_H