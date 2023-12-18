/**
 * Clasp Abstract Syntax Tree Implementation
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

#include <clasp/ast.h>
#include <stdlib.h>
#include <stdio.h>

ClaspASTNode *new_node(ClaspASTNodeType t, union ASTNodeData *data) {
    ClaspASTNode *node = malloc(sizeof(ClaspASTNode));
    node->type = t;
    node->data = *data;
    return node;
}


ClaspASTNode *binop(ClaspASTNode *left, ClaspASTNode *right, ClaspToken *op) {
    union ASTNodeData data = {
        .binop = {
            .left = left,
            .op = op,
            .right = right
        }
    };
    return new_node(AST_EXPR_BINOP, &data);
}
ClaspASTNode *unop(ClaspASTNode *right, ClaspToken *op) {
    union ASTNodeData data = {
        .unop = {
            .op = op,
            .right = right
        }
    };
    return new_node(AST_EXPR_UNOP, &data);
}
ClaspASTNode *lit_num(ClaspToken *n) {
    union ASTNodeData data = {
        .lit_num = {
            .value = n
        }
    };
    return new_node(AST_EXPR_LIT_NUMBER, &data);
}

void *visit(ClaspASTNode *node, ClaspASTVisitor v) {
    if (node->type < 0 || node->type > CLASP_NUM_VISITORS) {
        fprintf(stderr, "Internal error, please report this message: \n\n\"Unknown AST node type: %d\"\n", node->type);
        exit(1);
    }
    return v[node->type](node);
}