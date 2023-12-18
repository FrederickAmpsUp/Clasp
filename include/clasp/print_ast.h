/**
 * Clasp AST printer declaration
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

#ifndef PRINT_AST_H
#define PRINT_AST_H

#include <clasp/ast.h>

/**
 * Visitor function for printing binary operations.
 * @param binop The binary operation node
*/
void *claspPrintBinop(ClaspASTNode *binop);
/**
 * Visitor function for printing unary operations.
 * @param unop The unary operation node
*/
void *claspPrintUnop(ClaspASTNode *unop);
/**
 * Visitor expression for printing number literals.
 * @param lit The number literal node
*/
void *claspPrintNumLiteral(ClaspASTNode *lit);

/**
 * Print an Abstract Syntax Tree.
 * @param ast The tree to print
*/
void claspPrintAST(ClaspASTNode *ast);

// List of visitors.
extern ClaspASTVisitor clasp_ast_printer;

#endif // PRINT_AST_H