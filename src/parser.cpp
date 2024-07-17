#include <clasp/parser.hpp>
#include <clasp/ast.hpp>
#include <clasp/exception.hpp>

namespace clasp::ast {

BaseExpression::Ptr Parser::expression() {
    return assignment();
}

BaseStatement::Ptr Parser::statement() {
    // TODO: declstatements

    BaseExpression::Ptr expr = expression();

    clasp::lexical::Token::Ptr tok = clasp::lexical::Token::make_ptr(clasp::lexical::Token::Type::UNKNOWN);
    if (!consume({ clasp::lexical::Token::Type::SEMICOLON })) throw clasp::error::SyntaxError("Missing semicolon after expression - expected ';', got ", tok);

    return ExpressionStatement::make_ptr(expr);
}

// assignment is right-associative
BaseExpression::Ptr Parser::assignment() {
    BaseExpression::Ptr left = equality();

    clasp::lexical::Token::Ptr op = clasp::lexical::Token::make_ptr(clasp::lexical::Token::Type::UNKNOWN);
    if (consume({
        clasp::lexical::Token::Type::EQUAL,
        clasp::lexical::Token::Type::PLUS_EQUAL,
        clasp::lexical::Token::Type::MINUS_EQUAL,
        clasp::lexical::Token::Type::SLASH_EQUAL,
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
        clasp::lexical::Token::Type::EQUAL_EQUAL,
        clasp::lexical::Token::Type::BANG_EQUAL,
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
        clasp::lexical::Token::Type::LESS,
        clasp::lexical::Token::Type::GREATER,
        clasp::lexical::Token::Type::LESS_EQUAL,
        clasp::lexical::Token::Type::LESS_EQUAL,
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

BaseExpression::Ptr Parser::factor() {
    BaseExpression::Ptr left = exponent();

    clasp::lexical::Token::Ptr op = clasp::lexical::Token::make_ptr(clasp::lexical::Token::Type::UNKNOWN);
    while (consume({
        clasp::lexical::Token::Type::ASTERISK,
        clasp::lexical::Token::Type::SLASH,
        clasp::lexical::Token::Type::PERCENT,
    }, op)) {
        BaseExpression::Ptr right = exponent();

        left = BinaryExpression::make_ptr(left, op, right);
    }

    return left;
}

// right-associative
BaseExpression::Ptr Parser::exponent() {
    BaseExpression::Ptr left = unary();

    clasp::lexical::Token::Ptr op = clasp::lexical::Token::make_ptr(clasp::lexical::Token::Type::UNKNOWN);
    if (consume({
        clasp::lexical::Token::Type::CARET,
    }, op)) {
        BaseExpression::Ptr right = exponent();

        return BinaryExpression::make_ptr(left, op, right);
    }
    return left;
}

BaseExpression::Ptr Parser::unary() {
    clasp::lexical::Token::Ptr op = clasp::lexical::Token::make_ptr(clasp::lexical::Token::Type::UNKNOWN);
    if (consume({
        clasp::lexical::Token::Type::MINUS,
        clasp::lexical::Token::Type::BANG,
        clasp::lexical::Token::Type::TILDE,
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

    if (consume({
        clasp::lexical::Token::Type::LEFT_PAREN
    })) {
        BaseExpression::Ptr nested = expression();
        if (!consume({
            clasp::lexical::Token::Type::RIGHT_PAREN
        })) {
            tok = this->lexer_.peek();
            throw clasp::error::SyntaxError("Unexpected token - required ')', got ", tok);
        }
        return nested;
    }

    if (consume({
        clasp::lexical::Token::Type::IDENTIFIER
    }, tok)) {
        return VariableReference::make_ptr(tok);
    }
    
    tok = this->lexer_.peek();
    throw clasp::error::SyntaxError("Unexpected token whilst parsing primary expression: ", tok);
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