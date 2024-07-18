#pragma once

#include <variant>
#include <clasp/lexical.hpp>
#include <clasp/util.hpp>

namespace clasp::type {
struct PrimaryType;
struct Type {
    CLASP_UTIL_CLASS_PTR(Type)
};

struct PrimaryType : public Type {
    CLASP_UTIL_CLASS_PTR(PrimaryType)
    PrimaryType(clasp::lexical::Token::Ptr name) : name(name) {}

    clasp::lexical::Token::Ptr name;
};

struct PrimitiveType : public Type {
    CLASP_UTIL_CLASS_PTR(PrimitiveType)
    enum class Type {
        INT, UINT, FLOAT
    };

    PrimitiveType(Type type, uint8_t bits) : type(type), bits(bits) {}

    uint8_t bits;
    Type type;
};

// TODO: array types, function types, template types (?)
}