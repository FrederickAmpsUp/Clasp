/**
 * Clasp String Stream declaration
 * Authored 12/2023
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

#ifndef STRINGSTREAM_H
#define STRINGSTREAM_H

#include <clasp/lexer.h>

/**
 * String Stream. This store the data and index into it.
*/
typedef struct {
    char *data;
    unsigned int idx;
} StringStream;

/**
 * Allocate and intialize a new stream.
 * @param str The string to treat as a file
*/
StringStream* new_stream(char *str);

/**
 * Read a character from a stream.
 * @param s The stream to read from.
*/
char stream_read(StringStream *s);

#endif // STRINGSTREAM_H