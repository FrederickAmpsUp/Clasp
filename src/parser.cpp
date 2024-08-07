#include <clasp/parser.hpp>
#include <clasp/ast.hpp>
#include <clasp/exception.hpp>

namespace clasp::ast {

BaseExpression::Ptr Parser::expression() {
    return assignment();
}

BaseStatement::Ptr Parser::statement() {
    clasp::lexical::Token::Ptr tok = clasp::lexical::Token::make_ptr(clasp::lexical::Token::Type::UNKNOWN);
    if (consume({
        clasp::lexical::Token::Type::KEYWORD_VAR,
        clasp::lexical::Token::Type::KEYWORD_LET,
        clasp::lexical::Token::Type::KEYWORD_CONST,
    }, tok)) {
        clasp::lexical::Token::Ptr name = clasp::lexical::Token::make_ptr(clasp::lexical::Token::Type::UNKNOWN);
        if (!consume({
            clasp::lexical::Token::Type::IDENTIFIER
        }, name)) throw clasp::error::SyntaxError("Missing variable name - expected (IDENTIFIER), got ", this->lexer_.peek());
        VarDeclStatement::Qualifiers q;
        switch (tok->type) {
            case clasp::lexical::Token::Type::KEYWORD_VAR:
                q = VarDeclStatement::Qualifiers::MUTABLE;
                break;
            case clasp::lexical::Token::Type::KEYWORD_LET:
                q = VarDeclStatement::Qualifiers::IMMUABLE;
                break;
            case clasp::lexical::Token::Type::KEYWORD_CONST:
                q = VarDeclStatement::Qualifiers::CONST;
                break;
            // default is unreachable
        }

        BaseExpression::Ptr initializer = nullptr;
        clasp::type::Type::Ptr type = nullptr;

        if (consume({
            clasp::lexical::Token::Type::COLON
        })) {
            // TODO: type system
            type = this->parse_type();
        }

        if (consume({
            clasp::lexical::Token::Type::EQUAL
        })) {
            initializer = expression();
        }

        if (type == nullptr) {
            type = initializer->type();
        }
        
        if (!consume({ clasp::lexical::Token::Type::SEMICOLON })) throw clasp::error::SyntaxError("Missing semicolon after variable declaration - expected ';', got ", this->lexer_.peek());

        return VarDeclStatement::make_ptr(name->value, q, type, initializer);
    }

    BaseExpression::Ptr expr = expression();

    if (!consume({ clasp::lexical::Token::Type::SEMICOLON })) throw clasp::error::SyntaxError("Missing semicolon after expression - expected ';', got ", this->lexer_.peek());

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
            // i32 by default
        return NumberLiteral::make_ptr(tok, clasp::type::PrimitiveType::make_ptr(clasp::type::PrimitiveType::Type::INT, 32));
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

static const std::unordered_map<std::string, clasp::type::PrimitiveType::Ptr> primitives = {
    { "i8" , clasp::type::PrimitiveType::make_ptr(clasp::type::PrimitiveType::Type::INT, 8 ) },
    { "i16", clasp::type::PrimitiveType::make_ptr(clasp::type::PrimitiveType::Type::INT, 16) },
    { "i32", clasp::type::PrimitiveType::make_ptr(clasp::type::PrimitiveType::Type::INT, 32) },

    { "u8" , clasp::type::PrimitiveType::make_ptr(clasp::type::PrimitiveType::Type::UINT, 8 ) },
    { "u16", clasp::type::PrimitiveType::make_ptr(clasp::type::PrimitiveType::Type::UINT, 16) },
    { "u32", clasp::type::PrimitiveType::make_ptr(clasp::type::PrimitiveType::Type::UINT, 32) },

    { "f32", clasp::type::PrimitiveType::make_ptr(clasp::type::PrimitiveType::Type::FLOAT, 32) },
};

clasp::type::Type::Ptr Parser::parse_type() {
    // TODO: array and other types

    clasp::lexical::Token::Ptr tok = clasp::lexical::Token::make_ptr(clasp::lexical::Token::Type::UNKNOWN);
    if (!consume({
        clasp::lexical::Token::Type::IDENTIFIER
    }, tok)) throw clasp::error::SyntaxError("Missing type name - expected (IDENTIFIER), got ", this->lexer_.peek());

    if (primitives.find(tok->value) != primitives.end()) {
        return primitives.at(tok->value);
    }

    return clasp::type::PrimaryType::make_ptr(tok);
}
}