/**
 * Clasp String Stream declaration
 * Authored 1/9/2024
 * 
 * This program is part of the Clasp Header Libraries
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

#ifndef FSTREAM_H
#define FSTREAM_H

#include <clasp/lexer.h>
#include <stdio.h>

/**
 * File Stream. This store the data and index into it.
*/
typedef struct {
    FILE *file;
} FileStream;

/**
 * Allocate and intialize a new stream.
 * @param fname The file to open.
*/
FileStream* new_fstream(char *fname);

/**
 * Read a character from a stream.
 * @param s The stream to read from.
*/
char fstream_read(FileStream *s);

#endif // STRINGSTREAM_H