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

/**
 * Enumeration to store types of AST nodes.
 * CLASP_NUM_VISITORS must remain at the end of the list to accurately store the number of visitor function needed.
*/
typedef enum {
    AST_BINOP,
    AST_UNOP,
    AST_LIT_NUMBER,

    CLASP_NUM_VISITORS
} ClaspASTNodeType;

// Forware declaration
typedef struct ClaspASTNode ClaspASTNode;

/**
 * The actual data stored in AST nodes.
*/
union ASTNodeData {
    /**
     * Binary operations (5 + 3)
    */
    struct {
        ClaspASTNode *left;
        ClaspASTNode *right;
        ClaspToken *op;
    } binop;
    /**
     * Unary operations (-8)
    */
    struct {
        ClaspASTNode *right;
        ClaspToken *op;
    } unop;
    /**
     * Number literals (22)
    */
    struct {
        ClaspToken *value;
    } lit_num;
};

/**
 * Abstract Syntax Tree node.
 * This is essentially a tagged union that stores its type and all the associated data.
*/
typedef struct ClaspASTNode {
    ClaspASTNodeType type;
    union ASTNodeData data;
} ClaspASTNode;

/**
 * Allocate and initialize an AST node.
 * @param type The type of the new node.
 * @param data The data of the new node.
*/
ClaspASTNode *new_node(ClaspASTNodeType type, union ASTNodeData *data);

/**
 * Helper function for creating a binary op node.
 * @param left The left operand.
 * @param right The right operand.
 * @param op The binary operator to use.
*/
ClaspASTNode *binop(ClaspASTNode *left, ClaspASTNode *right, ClaspToken *op);

/**
 * Helper function for creating a unary op node.
 * @param right The right operand.
 * @param op The unary operator to use.
*/
ClaspASTNode *unop(ClaspASTNode *right, ClaspToken *op);

/**
 * Helper function for creating a number literal node.
 * @param num The number literal token to use.
*/
ClaspASTNode *lit_num(ClaspToken *num);

/**
 * AST visitor that can return data.
*/
typedef void *(*ClaspVisitorFn) (ClaspASTNode *node);

/**
 * List of visitors.
*/
typedef ClaspVisitorFn ClaspASTVisitor[CLASP_NUM_VISITORS];

/**
 * Helper function to call the appropriate visitor for the given node.
 * @param node The node to visit
 * @param visitor The visitor table to use.
*/
void *visit(ClaspASTNode *node, ClaspASTVisitor visitor);

#endif // AST_H