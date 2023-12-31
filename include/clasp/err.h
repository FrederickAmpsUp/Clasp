/**
 * Clasp error message declarations.
 * Authored 12/18/2023-present
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

#ifndef ERR_H
#define ERR_H

#include <clasp/lexer.h>

/**
 * Raise a general error. This does NOT exit the program.
 * @param fmt The format string to use.
*/
void general_err(const char *fmt, ...);

/**
 * Raise an error on a token. This does NOT exit the program.
 * @param tok The token in error.
 * @param err The error message.
*/
void token_err(ClaspToken *tok, char *err);

#endif // ERR_H