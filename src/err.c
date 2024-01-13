/**
 * Clasp error message implementations
 * Authored 12/18/2023-present
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

#include <clasp/err.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

static bool term_does_color() {
    const char *term = getenv("TERM");
    if (!term || strcmp(term, "dumb") == 0) {
        // "dumb" indicates a basic terminal with no color support
        return false;
    }

    if (isatty(fileno(stdout))) {
        return true;  // Terminal supports color
    } else {
        return false; // Terminal does not support color
    }
}

void general_err(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);
    return;
}

// yes, it's terrible
void token_err(ClaspToken *tok, char *err) {
    int startIdx = 0;
    if (startIdx < 0) startIdx = 0;
    int endIdx = strlen(tok->line);
    if (endIdx > strlen(tok->line)) endIdx = strlen(tok->line);
    int tokLen = strlen(tok->data);

    bool col = term_does_color();

    fprintf(stderr, "Syntax error in file %s, line %d:%d.\n", "TODO", tok->lineno + 1, tok->where + 1);
    for (unsigned int i = startIdx; i < endIdx; ++i) {
        if (col && (i == tok->where - tokLen)) printf("\033[1;31m");
        putchar(tok->line[i]);
        if (col && (i == tok->where)) printf("\033[0m");
    } putchar('\n');
    printf("\033[0m");

    int nSpaces = (int)tok->where - startIdx - tokLen - 2;
    while (isspace(tok->line[nSpaces + startIdx])) nSpaces--;
    if (nSpaces > 0)
        for (int i = 0; i <= nSpaces; ++i) {
            putchar(' ');
        }

    if (col) printf("\033[1;31m");
    for (int i = 0; i < strlen(tok->data); ++i)
        putchar('^');
    if (col) printf("\033[0m");

    printf("\n%s\n", err);

}