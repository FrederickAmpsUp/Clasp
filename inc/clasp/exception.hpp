#pragma once

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
}