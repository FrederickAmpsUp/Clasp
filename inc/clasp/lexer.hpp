#ifndef _INC_CLASP_LEXER_HPP
#define _INC_CLASP_LEXER_HPP

#include <string>
#include <memory>
#include <vector>

#include <istream>

namespace clasp::lexical {

struct Token {
    enum Type {
        PLUS, MINUS,
        ASTERIX, SLASH,

        PLUS_EQUAL, MINUS_EQUAL,
        ASTERIX_EQUAL, SLASH_EQUAL,

        PLUS_PLUS, MINUS_MINUS,

            // TODO: way more of these
        END_OF_FILE, NONE
    };

    struct Location {
        Location(std::string file, uint32_t line, uint64_t col) { this->file = file; this->line = line; this->col = col; };
        std::string file;
        uint32_t line;
        uint64_t col;

        using Ptr = std::shared_ptr<Location>;
        static Ptr New(std::string file="", uint32_t line=0, uint64_t col=0) { return std::make_shared<Location>(file, line, col); }
    };
    Token(Type type, std::string value, Location::Ptr where = nullptr) { this->type = type; this->value = value; this->where = where; }

    std::string value;
    Type type;
    Location::Ptr where;

    using Ptr = std::shared_ptr<Token>;
    static Ptr New(Type type, std::string value, Location::Ptr where = nullptr) { return std::make_shared<Token>(type, value, where); }

    std::string to_string() const;
};

class Analyzer {
public:
    Analyzer(std::istream& input) : _input(input) {}

    Token::Ptr scan();

    char peek();
    char next();
private:
    std::istream& _input;
    std::vector<Token::Ptr> tokens;
};

}

#endif /* _INC_CLASP_LEXER_HPP */