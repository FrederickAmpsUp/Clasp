#pragma once

// organized includes :)
#include <memory>
#include <utility>
#include <vector>
#include <string>
#include <istream>
#include <clasp/util.hpp>

/**
 * Lexical/tokenization data
 */
namespace clasp::lexical {
/**
 * A lexical token in the Clasp language
 */
struct Token {
// implied
public:
    /**
     * Smart pointer shorthand, so I have to use `auto` less
     */
    using Ptr = std::shared_ptr<Token>;

    /**
     * Construct a smart pointer to a token given the constructor arguments
     */
    template <typename ...Args>
    static Ptr make_ptr(Args&&... args) {
        return std::make_shared<Token>(std::forward<Args>(args)...);
    }

    /**
     * Scoped enumeration representing the types a lexical token can be
     */
    enum class Type {
        PLUS, MINUS, ASTERISK, SLASH,
        PLUS_EQUAL, MINUS_EQUAL,
        ASTERISK_EQUAL, SLASH_EQUAL,

        PLUS_PLUS, MINUS_MINUS,

        COLON, SEMICOLON,

        IDENTIFIER, INTEGER_LITERAL,

        END_OF_FILE,
        UNKNOWN
    };

    /**
     * Constructor
     * @param type The type of the lexical token
     * @param value The value or data of the lexical token. If not provided, the value will be generated based on the type, or left empty if not possible.
     */
    Token(Type type, std::string value="");

    Type type;
    std::string value;

    // TODO: location data
};

/**
 * Lexical analyzer/tokenizer/lexer/scanner for Clasp lexical tokens
 */
class Scanner {
public:
    /**
     * Smart pointer shorthand
     */
    using Ptr = std::shared_ptr<Scanner>;
    

    /**
     * Construct a smart pointer to a scanner given the constructor arguments
     */
    template <typename ...Args>
    static Ptr make_ptr(Args&&... args) {
        return std::make_shared<Scanner>(std::forward<Args>(args)...);
    }

    Scanner(std::istream& input) : input_(input) {
        (void) this->next(); // step to the first character
    }

    /**
     * Get the internal input stream
     * @retval The handle to the lexer's input stream
     */
    std::istream& input() { return this->input_; }

    /**
     * Get the list of output lexical tokens
     * @retval The list of output tokens
     */
    std::vector<Token::Ptr>& output() { return this->output_; }

    Token::Ptr scan();
private:
    std::istream& input_;
    std::vector<Token::Ptr> output_;

    char prev_;
    char cur_;
    char peek_;

    char next() {
        this->prev_ = this->cur_;
        this->cur_ = this->peek_;
        this->peek_ = this->input_.get();
        return this->cur_;
    }

    char consume(char c) {
        if (this->peek_ == c) {
            return this->next();
        }
        return 0;
    }
};
}