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

void *claspPrintBinop(ClaspASTNode *binop);
void *claspPrintUnop(ClaspASTNode *unop);
void *claspPrintNumLiteral(ClaspASTNode *lit);

void *claspPrintAST(ClaspASTNode *ast);

extern ClaspASTVisitor clasp_ast_printer;

#endif // PRINT_AST_H