/**
 * Clasp String Stream Implementation
 * Authored 12/2023
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

#include <clasp/stringstream.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

StringStream *new_stream(char *str) {
    char *new = malloc(strlen(str) + 1);
    strcpy(new, str);
    new[strlen(new)-1] = '\0';

    StringStream *stream = malloc(sizeof(StringStream));
    stream->data = new;
    stream->idx = 0;

    return stream;
}

char stream_read(StringStream *s) {
    if (s->idx >= strlen(s->data)) return EOF;
    return s->data[s->idx++];
}