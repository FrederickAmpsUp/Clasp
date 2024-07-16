#pragma once

#include <clasp/lexical.hpp>
#include <stdexcept>
#include <string>

namespace clasp::error {

class UnknownTokenError : public std::exception {
public:
        // TODO: location info
    UnknownTokenError(const std::string& message) : message_(message) {}

    virtual const char *what() const noexcept override { return message_.c_str(); }
private:
    std::string message_;
};

class SyntaxError : public std::exception {
public:
        // TODO: location info
    SyntaxError(const std::string& message, clasp::lexical::Token::Ptr token=nullptr) : message_(message), token_(token) {}

    virtual const char *what() const noexcept override {
        std::string msg = "\"" + message_ + "\" on token ";
        if (token_)
            msg += "'" + token_->value + "'";
        else msg += "(null)";

        return msg.c_str();
    }
private:
    std::string message_;
    clasp::lexical::Token::Ptr token_;
};
}