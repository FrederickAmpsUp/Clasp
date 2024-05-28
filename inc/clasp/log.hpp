#ifndef _INC_CLASP_LOG_HPP
#define _INC_CLASP_LOG_HPP

#include <string>
#include <stdexcept>

#include <clasp/lexer.hpp>

namespace clasp {

#if !defined(NDEBUG)

    // logging functions

#else

    // logging dummy functions

#endif /* !defined(NDEBUG) */

class TokenError : public std::runtime_error {
public:
    TokenError(clasp::lexical::Token::Location::Ptr where, std::string what) : _where(where), std::runtime_error(what) {}

    clasp::lexical::Token::Location::Ptr where() const { return _where; }

    virtual std::string why() const { return "Error"; }
private:
    clasp::lexical::Token::Location::Ptr _where;
};

class SyntaxError : public TokenError {
public:
        // inherited constructors
    using TokenError::TokenError;

    virtual std::string why() const override { return "SyntaxError"; }
private:
};

}

#endif /* _INC_CLASP_LOG_HPP */