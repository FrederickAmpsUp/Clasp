#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <memory>

namespace Clasp::Scanner {

struct Token {
    enum Type {
        IDENTIFIER,

        INTEGER_LITERAL, REAL_LITERAL,

        END_OF_FILE, NONE
    };

    static std::string typeToString(Type type);

    struct Location {
        std::string file;

        uint32_t lineno;
        uint32_t column;

        inline std::string toString() const {
            return "file: " + this->file + ", location: " + std::to_string(this->lineno) + ":" + std::to_string(this->column);
        }
    };

    Token::Type type;
    std::string data;

    std::shared_ptr<Token::Location> loc;
    std::string location() const;
    std::string toString() const;
};

class Scanner {
public:
    Scanner(std::shared_ptr<std::istream>& stream) : _source(stream) {}

    std::shared_ptr<std::istream>& source() const { return _source; }

    std::shared_ptr<Token> scan();

    std::vector<std::shared_ptr<Token>> scannedTokens() const { return _out; }
private:
    char getChar();

    std::shared_ptr<std::istream>& _source;
    std::vector<std::shared_ptr<Token>> _out;
};
};