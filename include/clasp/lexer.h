#ifndef LEXER_H
#define LEXER_H

typedef char (*StreamReadFn) ();

typedef enum {
    TOKEN_UNKNOWN
} TokenType;

typedef struct {
    char *const data;
    TokenType type;
} ClaspToken;

typedef struct {
    StreamReadFn stream;

    ClaspToken *current;
    ClaspToken *previous;
    ClaspToken *next;
} ClaspLexer;

void new_lexer(ClaspLexer *lexer, StreamReadFn fn);
ClaspToken *lexer_next(ClaspLexer *lexer);
ClaspToken *lexer_scan(ClaspLexer *lexer);

#endif // LEXER_H