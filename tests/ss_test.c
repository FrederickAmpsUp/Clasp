/**
 * Clasp String Stream test
 * Authored 12/2023
 * 
 * This program is part of the Clasp Test Suite
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
 *  Passes: (output matches input) as of 12/15/2023
*/

#include <clasp/lexer.h>
#include <clasp/stringstream.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

int main(int argc, char **argv) {
    StringStream *str = new_stream("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890  \n\nabcd");
    
    char c;
    while ((c = stream_read(str)) != EOF) {
        putchar(c);
    }
    putchar('\n');

    return 0;
}