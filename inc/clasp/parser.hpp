#pragma once

#include <clasp/ast.hpp>
#include <clasp/util.hpp>
#include <initializer_list>
#include <clasp/lexical.hpp>

namespace clasp::ast {

class Parser {
public:
    Parser(clasp::lexical::Scanner& lexer) : lexer_(lexer) {}

    BaseExpression::Ptr expression();
private:
    clasp::lexical::Scanner& lexer_;

protected:
    BaseExpression::Ptr assignment();
    BaseExpression::Ptr equality();
    BaseExpression::Ptr comparison();
    BaseExpression::Ptr term();
    BaseExpression::Ptr factor();
    BaseExpression::Ptr exponent();
    BaseExpression::Ptr unary();
        // TODO: postfix
    BaseExpression::Ptr primary();

    bool consume(std::initializer_list<clasp::lexical::Token::Type> types, clasp::lexical::Token::Ptr tok = nullptr);
};
}