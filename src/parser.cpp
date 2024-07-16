#include <clasp/parser.hpp>
#include <clasp/ast.hpp>
#include <clasp/exception.hpp>

namespace clasp::ast {

    // this will eventually expand to `assignment`
BaseExpression::Ptr Parser::expression() {
    return assignment();
}

// assignment is right-associative
BaseExpression::Ptr Parser::assignment() {
    BaseExpression::Ptr left = equality();

    clasp::lexical::Token::Ptr op = clasp::lexical::Token::make_ptr(clasp::lexical::Token::Type::UNKNOWN);
    if (consume({
        // TODO: assignment operators
    }, op)) {
        BaseExpression::Ptr right = assignment();

        return BinaryExpression::make_ptr(left, op, right);
    }
    return left;
}

BaseExpression::Ptr Parser::equality() {
    BaseExpression::Ptr left = comparison();

    clasp::lexical::Token::Ptr op = clasp::lexical::Token::make_ptr(clasp::lexical::Token::Type::UNKNOWN);
    while (consume({
        // TODO: equality operators
    }, op)) {
        BaseExpression::Ptr right = comparison();

        left = BinaryExpression::make_ptr(left, op, right);
    }
    return left;
}

BaseExpression::Ptr Parser::comparison() {
    BaseExpression::Ptr left = term();

    clasp::lexical::Token::Ptr op = clasp::lexical::Token::make_ptr(clasp::lexical::Token::Type::UNKNOWN);
    while (consume({
        // TODO: comparison operators
    }, op)) {
        BaseExpression::Ptr right = term();

        left = BinaryExpression::make_ptr(left, op, right);
    }
    return left;
}

BaseExpression::Ptr Parser::term() {
    BaseExpression::Ptr left = factor();

    clasp::lexical::Token::Ptr op = clasp::lexical::Token::make_ptr(clasp::lexical::Token::Type::UNKNOWN);
    while (consume({
        clasp::lexical::Token::Type::PLUS,
        clasp::lexical::Token::Type::MINUS,
    }, op)) {
        BaseExpression::Ptr right = factor();

        left = BinaryExpression::make_ptr(left, op, right);
    }

    return left;
}

    // will reference exponent() in the future
BaseExpression::Ptr Parser::factor() {
    BaseExpression::Ptr left = unary();

    clasp::lexical::Token::Ptr op = clasp::lexical::Token::make_ptr(clasp::lexical::Token::Type::UNKNOWN);
    while (consume({
        clasp::lexical::Token::Type::ASTERISK,
        clasp::lexical::Token::Type::SLASH,
    }, op)) {
        BaseExpression::Ptr right = unary();

        left = BinaryExpression::make_ptr(left, op, right);
    }

    return left;
}

BaseExpression::Ptr Parser::unary() {
    clasp::lexical::Token::Ptr op = clasp::lexical::Token::make_ptr(clasp::lexical::Token::Type::UNKNOWN);
    if (consume({
        clasp::lexical::Token::Type::MINUS,
    }, op)) {
        BaseExpression::Ptr right = unary();

        return UnaryExpression::make_ptr(op, right);
    }
    return primary();
}

BaseExpression::Ptr Parser::primary() {
    clasp::lexical::Token::Ptr tok = clasp::lexical::Token::make_ptr(clasp::lexical::Token::Type::UNKNOWN);
    if (consume({
        clasp::lexical::Token::Type::INTEGER_LITERAL
    }, tok)) {
        return NumberLiteral::make_ptr(tok);
    }
    tok = this->lexer_.peek();

    throw clasp::error::SyntaxError("Syntax error while parsing primary expression", tok);
}

bool Parser::consume(std::initializer_list<clasp::lexical::Token::Type> types, clasp::lexical::Token::Ptr tok) {
    for (const auto& type : types) {
        if (type == this->lexer_.peek()->type) {
                auto t = this->lexer_.scan();
                if (tok) {
                    *tok = *t;
                }
                return true;
        }
    }
    return false;
}
}