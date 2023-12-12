#ifndef LEXER_H
#define LEXER_H

typedef char (*StreamReadFn) ();

// TODO: file/line/column numbers
// in tokens (for debugging)

typedef enum {
    TOKEN_ID, TOKEN_NUMBER,
    TOKEN_KW_IF,TOKEN_KW_WHILE,
    TOKEN_KW_FOR,TOKEN_KW_FN,

    TOKEN_PLUS,TOKEN_MINUS,
    TOKEN_ASTERIX,TOKEN_SLASH,

    TOKEN_EQ_EQ,
    TOKEN_BANG,TOKEN_BANG_EQ,
    TOKEN_LESS,TOKEN_LESS_EQ,
    TOKEN_GREATER,TOKEN_GREATER_EQ,

    TOKEN_EQ,
    TOKEN_PLUS_EQ,TOKEN_MINUS_EQ,
    TOKEN_ASTERIX_EQ,TOKEN_SLASH_EQ,

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
} ClaspLexer;

void new_lexer(ClaspLexer *lexer, StreamReadFn fn);
ClaspToken *lexer_next(ClaspLexer *lexer);
ClaspToken *lexer_scan(ClaspLexer *lexer);
int lexer_has(ClaspLexer *lexer, ClaspTokenType type);

const char *toktyp_str(ClaspTokenType type);
void token_print(ClaspToken *token);

#endif // LEXER_H