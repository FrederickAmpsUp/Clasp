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
#include <cvector/cvector.h>

/**
 * Enumeration to store types of AST nodes.
 * CLASP_NUM_VISITORS must remain at the end of the list to accurately store the number of visitor function needed.
*/
typedef enum {
    AST_EXPR_BINOP,
    AST_EXPR_UNOP,
    AST_EXPR_POSTFIX,
    AST_EXPR_LIT_NUMBER,
    AST_EXPR_VAR_REF,
    AST_EXPR_FN_CALL,

    AST_EXPR_STMT,
    AST_BLOCK_STMT,
    AST_VAR_DECL_STMT,
    AST_LET_DECL_STMT,
    AST_CONST_DECL_STMT,
    AST_FN_DECL_STMT,

    CLASP_NUM_VISITORS
} ClaspASTNodeType;

// Forward declaration
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
     * Postfix (x++)
    */
    struct {
        ClaspASTNode *left;
        ClaspToken *op;
    } postfix;
    /**
     * Number literals (22)
    */
    struct {
        ClaspToken *value;
    } lit_num;
    /**
     * Variable references (x, foo)
    */
    struct {
        ClaspToken *varname;
    } var_ref;
    /**
     * Function calls (foo(), mul(a,b))
    */
    struct {
        ClaspASTNode *referencer;
        cvector(ClaspASTNode *) args;
    } fn_call;
    /**
     * Expression statements (see syntax.md)
    */
    struct {
            ClaspASTNode *expr;
    } expr_stmt;
    /**
     * Block statment (see syntax.md)
    */
    struct {
        cvector(ClaspASTNode *) body;
    } block_stmt;
    // TODO: other statments
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
ClaspASTNode *new_AST_node(ClaspASTNodeType type, union ASTNodeData *data);

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
 * Helper function for creating a postfix op node.
 * @param left The left operand.
 * @param op The postix operator to use.
*/
ClaspASTNode *postfix(ClaspASTNode *left, ClaspToken *op);

/**
 * Helper function for creating a number literal node.
 * @param num The number literal token to use.
*/
ClaspASTNode *lit_num(ClaspToken *num);

/**
 * Helper function for creating a variable reference node.
 * @param varname The name of the variable to reference.
*/
ClaspASTNode *var_ref(ClaspToken *varname);

/**
 * Helper function for creating a function call node.
 * @param referencer The object to call. This is usually a variable/fn name but can be any function type.
 * @param args The arguments to pass to the function. TODO: pass args by name instead of order.
*/
ClaspASTNode *fn_call(ClaspASTNode *referencer, cvector(ClaspASTNode *) args);


/**
 * Helper function for creating an expression statement node.
 * @param expr The expression.
*/
ClaspASTNode *expr_stmt(ClaspASTNode *expr);

/**
 * Helper function for creating a block statement node.
 * @param block The list of statements to use in the block.
*/
ClaspASTNode *block_stmt(cvector(ClaspASTNode *) block);

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