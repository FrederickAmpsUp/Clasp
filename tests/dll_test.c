/**
 * Test of the cross-platform DLL used for build targets.
 * Authored 1/4/2024
 * 
 * This program is part of the Clasp Test Suite
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

#include <stdlib.h>
#include <stdio.h>
#include <clasp/target.h>

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Usage: dll_test lib\n");
        return 1;
    }

    char *filename = argv[1];
    _DLL *lib = _DLL_open(filename);
    int (*add)(int a, int b) = _DLL_load(lib, int, "add");

    int a = 5;
    int b = 3;
    printf("%d + %d = %d\n", a, b, add(a,b));

    return 0;
}