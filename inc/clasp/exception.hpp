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
    SyntaxError(const std::string& message, clasp::lexical::Token::Ptr token=nullptr) : message_(message), token_(token) {
        aggregated_message_ += "SyntaxError: " + message_ + "'";
        if (token_)
            aggregated_message_ += token_->value;
        else
            aggregated_message_ += "(null)";
        aggregated_message_ += "'";
    }

    virtual const char *what() const noexcept override {
        return aggregated_message_.c_str();
    }
private:
    std::string message_;
    clasp::lexical::Token::Ptr token_;
        // must be a member variable to avoid deallocation (and likely corruption) after return from what()
    std::string aggregated_message_;
};
}