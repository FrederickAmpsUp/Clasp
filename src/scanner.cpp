#include <clasp/scanner.hpp>

namespace Clasp::Scanner {

char Scanner::getChar() {
    char c;
        // End of File
    if (!this->_source->get(c)) c = EOF;
    return c;
}

static bool isIdStart(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c == '_');
}
static bool isId(char c) {
    return isIdStart(c) || isdigit(c);
}

std::shared_ptr<Token> Scanner::scan() {
    Token result;

    char c = getChar();
    while (isspace((c))) c = getChar();
    result = Token{Token::Type::NONE, std::string(&c, 1), nullptr};
    if (c == EOF) { result.type = Token::Type::END_OF_FILE; goto end; }
    
        // Identifier
    if (isIdStart(c)) {
        std::string id;
        id += c;

        while (isId(c = getChar())) {
            id += c;
        }

        this->_source->seekg(-1, std::ios_base::cur); // go back 1 character

        result = Token{Token::Type::IDENTIFIER, id, nullptr};
        goto end;
    }

        // Number (may start with a decimal point)
    if (isdigit(c) || c == '.') {
        std::string num;
        num += c;

        while (isdigit(c = getChar()) || c == '.') {
            num += c;
        }

        this->_source->seekg(-1, std::ios_base::cur);

        result = Token{Token::Type::INTEGER_LITERAL, num, nullptr};

        if (num.find('.') != std::string::npos) {
            result.type = Token::Type::REAL_LITERAL;
        }

        goto end;
    }

end:
    // copy the result
    std::shared_ptr<Token> retval = std::make_shared<Token>(result);
    // add the token to the list of all scanned tokens
    this->_out.push_back(retval);
    return retval;
}

std::string Token::location() const {
    if (this->loc == nullptr) return "no location";
    return this->loc->toString();
}

std::string Token::toString() const {
    return "(" + Token::typeToString(this->type) + ": \"" + this->data + "\")";
}

std::string Token::typeToString(Type type) {
    switch (type) {
        case Type::INTEGER_LITERAL: return "INTEGER_LITERAL";
        case Type::REAL_LITERAL: return "REAL_LITERAL";
        case Type::IDENTIFIER: return "IDENTIFIER";
        case Type::END_OF_FILE: return "END_OF_FILE";
        case Type::NONE: return "NONE";
        default: return "UNKNOWN";
    }
}
};