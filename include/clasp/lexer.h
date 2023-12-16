/**
 * Clasp Lexical Analizer declaration
 * Authored 12/2023-present
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

#ifndef LEXER_H
#define LEXER_H

typedef char (*StreamReadFn) (void *);

// TODO: file/line/column numbers
// in tokens (for debugging)

typedef enum {
    TOKEN_ID, TOKEN_NUMBER,
    TOKEN_KW_IF,TOKEN_KW_WHILE,
    TOKEN_KW_FOR,TOKEN_KW_FN,

    TOKEN_PLUS,TOKEN_MINUS,
    TOKEN_ASTERIX,TOKEN_SLASH,
    TOKEN_PERC,TOKEN_CARAT,

    TOKEN_EQ_EQ,
    TOKEN_BANG,TOKEN_BANG_EQ,
    TOKEN_LESS,TOKEN_LESS_EQ,
    TOKEN_GREATER,TOKEN_GREATER_EQ,

    TOKEN_EQ,
    TOKEN_PLUS_EQ,TOKEN_MINUS_EQ,
    TOKEN_ASTERIX_EQ,TOKEN_SLASH_EQ,
    TOKEN_PERC_EQ,TOKEN_CARAT_EQ,

    TOKEN_LEFT_PAREN,TOKEN_RIGHT_PAREN,
    TOKEN_LEFT_SQUARE,TOKEN_RIGHT_SQUARE,
    TOKEN_LEFT_CURLY,TOKEN_RIGHT_CURLY,

    TOKEN_COLON,
    TOKEN_RIGHT_POINT,TOKEN_LEFT_POINT,

    TOKEN_EOF, TOKEN_UNKNOWN
} ClaspTokenType;

typedef struct {
    char *data;
    ClaspTokenType type;
} ClaspToken;

typedef struct {
    StreamReadFn stream;

    ClaspToken *current;
    ClaspToken *previous;
    ClaspToken *next;

    char cCurrent;
    void *_stream_args;
} ClaspLexer;

void new_lexer(ClaspLexer *lexer, StreamReadFn fn, void *args);
ClaspToken *lexer_next(ClaspLexer *lexer);
ClaspToken *lexer_scan(ClaspLexer *lexer);
int lexer_has(ClaspLexer *lexer, ClaspTokenType type);

const char *tktyp_str(ClaspTokenType type);
void token_print(ClaspToken *token);

#endif // LEXER_H