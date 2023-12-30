/**
 * Clasp language expression parser/AST visitor test
 * Authored 12/14/2023-present
 * 
 * This program is part of the Clasp Test Suite.
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

/**
 * Test status:
 *  Passes (output looks correct) as of 12/18/2023
*/

#include <clasp/lexer.h>
#include <clasp/parser.h>
#include <clasp/print_ast.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef struct {
    char *data;
    unsigned int idx;
} StringStream;

StringStream str;

char read_string() {
    if (str.idx == strlen(str.data)) return EOF;
    return str.data[str.idx++];
}

int main(int argc, char **argv) {
    str = (StringStream) { "foo(25 + 36);\nvar test: float = 8.5 / bar(3.2);\nlet piApprox: int = 3;\n", 0 };
    ClaspLexer *l = malloc(sizeof(ClaspLexer));
    new_lexer(l, read_string, NULL);
    ClaspParser *p = malloc(sizeof(ClaspParser));
    new_parser(p, l);
    ClaspASTNode *tree = parser_compile(p);
    claspPrintAST(tree);
    
    return 0;
}