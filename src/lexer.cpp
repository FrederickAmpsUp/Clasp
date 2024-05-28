#include <clasp/lexer.hpp>

#include <array>
#include <iostream>

namespace clasp::lexical {

std::string Token::to_string() const {

    return "Token(" + this->value + ")";
}

/* TODO: position info */
Token::Ptr Analyzer::scan() {
    char c = this->peek();

    while (isspace(c)) {
        (void) this->next();
        c = this->peek();
    }

    if (c == EOF) {
        return Token::New(Token::Type::END_OF_FILE, "\xff");
    }

    if (c == '+') {
        (void) this->next(); // advance head
        c = this->peek();
        if (c == '=') {
            (void) this->next(); // advance head
            return Token::New(Token::Type::PLUS_EQUAL, "+=");
        }
        if (c == '+') {
            (void) this->next(); // advance head
            return Token::New(Token::Type::PLUS_PLUS, "++");
        }
        return Token::New(Token::Type::PLUS, "+");
    }

    if (c == '-') {
        (void) this->next(); // advance head
        c = this->peek();
        if (c == '=') {
            (void) this->next(); // advance head
            return Token::New(Token::Type::MINUS_EQUAL, "-=");
        }
        if (c == '-') {
            (void) this->next(); // advance head
            return Token::New(Token::Type::MINUS_MINUS, "--");
        }
        return Token::New(Token::Type::MINUS, "-");
    }

    if (c == '*') {
        (void) this->next(); // advance head
        c = this->peek();
        if (c == '=') {
            (void) this->next(); // advance head
            return Token::New(Token::Type::ASTERIX_EQUAL, "*=");
        }
        return Token::New(Token::Type::ASTERIX, "*");
    }

    if (c == '/') {
        (void) this->next(); // advance head
        c = this->peek();
        if (c == '=') {
            (void) this->next(); // advance head
            return Token::New(Token::Type::SLASH_EQUAL, "/=");
        }
        return Token::New(Token::Type::SLASH, "/");
    }

    (void) this->next(); // advance head

    return Token::New(Token::Type::NONE, "");
}

char Analyzer::peek() {
    return this->_input.peek();
}

char Analyzer::next() {
    return this->_input.get();
}
};