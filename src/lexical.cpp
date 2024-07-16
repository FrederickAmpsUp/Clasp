#include <clasp/lexical.hpp>
#include <clasp/exception.hpp>
#include <tuple>

namespace clasp::lexical {

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

            case Type::COLON:
                value = ":";
                break;
            case Type::SEMICOLON:
                value = ";";
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
 */
static const std::tuple<char, Token::Type, Token::Type, Token::Type> operatorTokens[] = {
// operator     type                   op= type                     opop type
    { '+', Token::Type::PLUS,     Token::Type::PLUS_EQUAL,     Token::Type::PLUS_PLUS   },
    { '-', Token::Type::MINUS,    Token::Type::MINUS_EQUAL,    Token::Type::MINUS_MINUS },
    { '*', Token::Type::ASTERISK, Token::Type::ASTERISK_EQUAL, Token::Type::UNKNOWN     },
    { '/', Token::Type::SLASH,    Token::Type::SLASH_EQUAL,    Token::Type::UNKNOWN     },

    { ':', Token::Type::COLON,    Token::Type::UNKNOWN,        Token::Type::UNKNOWN     },
    { ';', Token::Type::SEMICOLON,Token::Type::UNKNOWN,        Token::Type::UNKNOWN     },
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