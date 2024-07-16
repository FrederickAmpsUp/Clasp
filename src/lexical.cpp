#include <clasp/lexical.hpp>
#include <clasp/exception.hpp>

namespace clasp::lexical {

Token::Token(Token::Type type, std::string value) {
    if (value.empty()) {
        switch (type) {
            case Type::PLUS:
                value = "+";
                break;

            case Type::MINUS:
                value = "-";
                break;

            case Type::ASTERISK:
                value = "*";
                break;

            case Type::SLASH:
                value = "/";
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

Token::Ptr Scanner::scan() {
    char c;

        // skip whitespace
    do {} while (isspace((c = this->next())));

    switch (c) {
        case '+':
            return Token::make_ptr(Token::Type::PLUS);
            
        case '-':
            return Token::make_ptr(Token::Type::MINUS);
            
        case '*':
            return Token::make_ptr(Token::Type::ASTERISK);
            
        case '/':
            return Token::make_ptr(Token::Type::SLASH);

        case EOF:
            return Token::make_ptr(Token::Type::END_OF_FILE);
    }

    throw clasp::error::UnknownTokenError("Unknown token while scanning (got character '" + std::string(&c, 1) + "')");
    return Token::make_ptr(Token::Type::UNKNOWN);
}
}