/**
 * Clasp Lexical Analizer declaration
 * Authored 12/2023-present
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

#ifndef LEXER_H
#define LEXER_H

/**
 * Function to read a character from a stream
*/
typedef char (*StreamReadFn) (void *);

// TODO: file/line/column numbers
// in tokens (for debugging)

/**
 * Enumeration of token types.
*/
typedef enum {
    TOKEN_ID, TOKEN_NUMBER,
    TOKEN_KW_IF,TOKEN_KW_WHILE,
    TOKEN_KW_FOR,TOKEN_KW_FN,
    TOKEN_KW_VAR,TOKEN_KW_LET,
    TOKEN_KW_CONST,

    TOKEN_PLUS,TOKEN_MINUS,
    TOKEN_ASTERIX,TOKEN_SLASH,
    TOKEN_PERC,TOKEN_CARAT,

    TOKEN_EQ_EQ,
    TOKEN_PLUS_PLUS, TOKEN_MINUS_MINUS,
    TOKEN_BANG,TOKEN_BANG_EQ,
    TOKEN_TILDE, TOKEN_TILDE_EQ,
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
    TOKEN_COMMA, TOKEN_SEMICOLON,

    TOKEN_EOF, TOKEN_UNKNOWN
} ClaspTokenType;

/**
 * Token that stores its type and data (the string it was derived from).
*/
typedef struct {
    char *data;
    ClaspTokenType type;

    char *line;
    unsigned int lineno;
    unsigned int where;
} ClaspToken;

/**
 * State of a lexer, stores all the neccesary tokens, the current stream character, and the input stream.
*/
typedef struct {
    StreamReadFn stream;

    ClaspToken *current;
    ClaspToken *previous;
    ClaspToken *next;

    char cCurrent;
    void *_stream_args;

    char **lines;
    unsigned int lineno;

    char *current_line;
    unsigned int col_idx;
} ClaspLexer;

/**
 * Initialize a new lexer.
 * @param lexer The lexer to initialize.
 * @param fn The stream function to be called to read a character.
 * @param args The arguments to be passed to the stream function.
*/
void new_lexer(ClaspLexer *lexer, StreamReadFn fn, void *args);

/**
 * Get the next token in the lexer's stream.
 * @param lexer The lexer to get the next token from.
*/
ClaspToken *lexer_next(ClaspLexer *lexer);

/**
 * Scan a token. This should only be used internally except special cases.
 * @param lexer The lexer to scan with.
*/
ClaspToken *lexer_scan(ClaspLexer *lexer);

/**
 * Check if a lexer's next token is of the given type.
 * @param lexer The lexer to check.
 * @param type The token type to check.
*/
int lexer_has(ClaspLexer *lexer, ClaspTokenType type);

/**
 * Helper function to convert a token type to a string.
 * @param type The type to stringify.
*/
const char *tktyp_str(ClaspTokenType type);

/**
 * Helper function to print a token with its type and data.
 * @param token The token to print.
*/
void token_print(ClaspToken *token);

#endif // LEXER_H