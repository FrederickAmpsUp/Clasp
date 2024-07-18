#include <clasp/lexical.hpp>
#include <clasp/exception.hpp>
#include <unordered_map>
#include <tuple>

namespace clasp::lexical {

const std::unordered_map<Token::Type, std::string> Token::typeStrings = {
    { Token::Type::PLUS, "PLUS" },
    { Token::Type::MINUS, "MINUS" },
    { Token::Type::ASTERISK, "ASTERISK" },
    { Token::Type::SLASH, "SLASH" },
    { Token::Type::CARET, "CARET" },
    { Token::Type::PERCENT, "PERCENT" },
    { Token::Type::TILDE, "TILDE" },
    { Token::Type::AMPERSAND, "AMPERSAND" },

    { Token::Type::PLUS_EQUAL, "PLUS_EQUAL" },
    { Token::Type::MINUS_EQUAL, "MINUS_EQUAL" },
    { Token::Type::ASTERISK_EQUAL, "ASTERISK_EQUAL" },
    { Token::Type::SLASH_EQUAL, "SLASH_EQUAL" },
    { Token::Type::CARET_EQUAL, "CARET_EQUAL" },
    { Token::Type::PERCENT_EQUAL, "PERCENT_EQUAL" },
    { Token::Type::TILDE_EQUAL, "TILDE_EQUAL" },
    { Token::Type::AMPERSAND_EQUAL, "AMPERSAND_EQUAL" },
    
    { Token::Type::PLUS_PLUS, "PLUS_PLUS" },
    { Token::Type::MINUS_MINUS, "MINUS_MINUS" },

    { Token::Type::EQUAL, "EQUAL" },
    { Token::Type::EQUAL_EQUAL, "EQUAL_EQUAL" },

    { Token::Type::BANG, "BANG" },
    { Token::Type::BANG_EQUAL, "BANG_EQUAL" },
    { Token::Type::GREATER, "GREATER" },
    { Token::Type::GREATER_EQUAL, "GREATER_EQUAL" },
    { Token::Type::LESS, "LESS" },
    { Token::Type::LESS_EQUAL, "LESS_EQUAL" },

    { Token::Type::COLON, "COLON" },
    { Token::Type::SEMICOLON, "SEMICOLON" },

    { Token::Type::LEFT_PAREN, "LEFT_PAREN" },
    { Token::Type::RIGHT_PAREN, "RIGHT_PAREN" },

    { Token::Type::IDENTIFIER, "IDENTIFIER" },
    { Token::Type::INTEGER_LITERAL, "INTEGER_LITERAL" },

    { Token::Type::KEYWORD_IF, "KEYWORD_IF" },
    { Token::Type::KEYWORD_WHILE, "KEYWORD_WHILE" },
    { Token::Type::KEYWORD_LET, "KEYWORD_LET" },
    { Token::Type::KEYWORD_VAR, "KEYWORD_VAR" },
    { Token::Type::KEYWORD_CONST, "KEYWORD_CONST" },
    { Token::Type::KEYWORD_FN, "KEYWORD_FN" },

    { Token::Type::END_OF_FILE, "END_OF_FILE" },
    { Token::Type::UNKNOWN, "UNKNOWN" },
};

Token::Token(Token::Type type, std::string value) {
    if (value.empty()) {
        switch (type) {
            case Type::PLUS:
                value = "+";
                break;
            case Type::PLUS_EQUAL:
                value = "+=";
                break;
            case Type::PLUS_PLUS:
                value = "++";
                break;

            case Type::MINUS:
                value = "-";
                break;
            case Type::MINUS_EQUAL:
                value = "-=";
                break;
            case Type::MINUS_MINUS:
                value = "--";
                break;

            case Type::ASTERISK:
                value = "*";
                break;
            case Type::ASTERISK_EQUAL:
                value = "*=";
                break;

            case Type::SLASH:
                value = "/";
                break;
            case Type::SLASH_EQUAL:
                value = "/=";
                break;

            case Type::CARET:
                value = "^";
                break;
            case Type::CARET_EQUAL:
                value = "^=";
                break;

            case Type::PERCENT:
                value = "%";
                break;
            case Type::PERCENT_EQUAL:
                value = "%=";
                break;

            case Type::TILDE:
                value = "~";
                break;
            case Type::TILDE_EQUAL:
                value = "~=";
                break;
                        
            case Type::AMPERSAND:
                value = "&";
                break;
            case Type::AMPERSAND_EQUAL:
                value = "&=";
                break;

            case Type::EQUAL:
                value = "=";
                break;
            case Type::EQUAL_EQUAL:
                value = "==";
                break;

            case Type::BANG:
                value = "!";
                break;
            case Type::BANG_EQUAL:
                value = "!=";
                break;

            case Type::LESS:
                value = "<";
                break;
            case Type::LESS_EQUAL:
                value = "<=";
                break;

            case Type::GREATER:
                value = ">";
                break;
            case Type::GREATER_EQUAL:
                value = ">=";
                break;

            case Type::COLON:
                value = ":";
                break;
            case Type::SEMICOLON:
                value = ";";
                break;

            case Type::LEFT_PAREN:
                value = "(";
                break;
            case Type::RIGHT_PAREN:
                value = ")";
                break;

            case Type::KEYWORD_IF:
                value = "if";
                break;
            case Type::KEYWORD_WHILE:
                value = "while";
                break;
            case Type::KEYWORD_VAR:
                value = "var";
                break;
            case Type::KEYWORD_LET:
                value = "let";
                break;
            case Type::KEYWORD_CONST:
                value = "const";
                break;
            case Type::KEYWORD_FN:
                value = "fn";
                break;

            case Type::END_OF_FILE:
                value = "\xff";
                break;

            default:
                value = "";
        }
    }

    this->value = std::move(value);
    this->type = type;
}


    // TODO: add more stuff

/**
 * Operator table for op, op=, and opop operators
 * could convert this to a hashmap
 */
static const std::tuple<char, Token::Type, Token::Type, Token::Type> operatorTokens[] = {
// operator     type                   op= type                     opop type
    { '+', Token::Type::PLUS,         Token::Type::PLUS_EQUAL,     Token::Type::PLUS_PLUS   },
    { '-', Token::Type::MINUS,        Token::Type::MINUS_EQUAL,    Token::Type::MINUS_MINUS },
    { '*', Token::Type::ASTERISK,     Token::Type::ASTERISK_EQUAL, Token::Type::UNKNOWN     },
    { '/', Token::Type::SLASH,        Token::Type::SLASH_EQUAL,    Token::Type::UNKNOWN     },

    { '^', Token::Type::CARET,        Token::Type::CARET_EQUAL,    Token::Type::UNKNOWN     },
    { '%', Token::Type::PERCENT,      Token::Type::PERCENT_EQUAL,  Token::Type::UNKNOWN     },
    { '~', Token::Type::TILDE,        Token::Type::TILDE_EQUAL,    Token::Type::UNKNOWN     },
    { '&', Token::Type::AMPERSAND,    Token::Type::AMPERSAND_EQUAL,Token::Type::UNKNOWN     },

    { '=', Token::Type::EQUAL,        Token::Type::EQUAL_EQUAL,    Token::Type::EQUAL_EQUAL },
    { '!', Token::Type::BANG,         Token::Type::BANG_EQUAL,     Token::Type::UNKNOWN     },
    { '<', Token::Type::LESS,         Token::Type::LESS_EQUAL,     Token::Type::UNKNOWN     },
    { '>', Token::Type::GREATER,      Token::Type::GREATER_EQUAL,  Token::Type::UNKNOWN     },

    { ':', Token::Type::COLON,        Token::Type::UNKNOWN,        Token::Type::UNKNOWN     },
    { ';', Token::Type::SEMICOLON,    Token::Type::UNKNOWN,        Token::Type::UNKNOWN     },
    
    { '(', Token::Type::LEFT_PAREN,   Token::Type::UNKNOWN,        Token::Type::UNKNOWN     },
    { ')', Token::Type::RIGHT_PAREN,  Token::Type::UNKNOWN,        Token::Type::UNKNOWN     },
};

/**
 * Keyword table [keyword, type]
 */
static const std::unordered_map<std::string, Token::Type> keywords = {
    { "if",    Token::Type::KEYWORD_IF    },
    { "while", Token::Type::KEYWORD_WHILE },
    { "var",   Token::Type::KEYWORD_VAR   },
    { "let",   Token::Type::KEYWORD_LET   },
    { "const", Token::Type::KEYWORD_CONST },
    { "fn",    Token::Type::KEYWORD_FN    }
};

// identifiers can start with _a-zA-Z
static bool isIdStart(char c) {
    return c == '_' || isalpha(c);
}
// identifier bodies consist of _a-zA-Z0-9
static bool isId(char c) {
    return isIdStart(c) || isdigit(c);
}

Token::Ptr Scanner::_scan() {
    char c;

        // skip whitespace
    do {} while (isspace((c = this->next())));

    if (c == EOF) return Token::make_ptr(Token::Type::END_OF_FILE);

    for (const auto& [ op, opType, opEqType, opOpType ] : operatorTokens) {
        if (c == op) {
            if (consume('=') && opEqType != Token::Type::UNKNOWN) return Token::make_ptr(opEqType);
            if (consume(op ) && opOpType != Token::Type::UNKNOWN) return Token::make_ptr(opOpType);
            return Token::make_ptr(opType);
        }
    }

    if (isIdStart(c)) {
        std::string id = std::string(&c, 1);

        while (isId(c = this->peek_)) {
            (void)this->next();  // consume
            id += c;
        }

            // O(1) :)
        if (keywords.find(id) != keywords.end()) {
            return Token::make_ptr(keywords.at(id), id);
        }
        
        return Token::make_ptr(Token::Type::IDENTIFIER, std::move(id));
    }

    if (isdigit(c)) {
        std::string num = std::string(&c, 1);
        while (isdigit(c = this->peek_)) {
            (void)this->next();  // consume
            num += c;
        }
        return Token::make_ptr(Token::Type::INTEGER_LITERAL, std::move(num));
    }

    throw clasp::error::UnknownTokenError("Unknown token whilst scanning (got character '" + std::string(&c, 1) + "')");
    return Token::make_ptr(Token::Type::UNKNOWN);
}

Token::Ptr Scanner::scan() {
    this->output_.push_back(this->_scan());
    if (this->output_.size() <= 1) {
        return nullptr;
    }
    return this->output_[this->output_.size() - 2];
}
}