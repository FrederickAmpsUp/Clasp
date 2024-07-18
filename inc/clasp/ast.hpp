#pragma once

#include <string>
#include <memory>
#include <clasp/util.hpp>
#include <clasp/lexical.hpp>
#include <clasp/type.hpp>

namespace clasp::ast {

class Visitor;

class BaseExpression {
public:
    BaseExpression(clasp::type::Type::Ptr type) : type_(type) {}

    CLASP_UTIL_CLASS_PTR(BaseExpression)

    virtual void accept(Visitor *v) = 0;

    clasp::type::Type::Ptr type() const { return type_; }
protected:
    clasp::type::Type::Ptr type_;
};

class BaseStatement {
public:
    CLASP_UTIL_CLASS_PTR(BaseStatement)

    virtual void accept(Visitor *v) = 0;
private:
};

class BinaryExpression;
class UnaryExpression;
class NumberLiteral;
class VariableReference;

class ExpressionStatement;
class VarDeclStatement;

class Visitor {
public:
    void visit(BaseExpression::Ptr e) { e->accept(this); }
    void visit(BaseStatement::Ptr e) { e->accept(this); }

    virtual void visit( BinaryExpression *e) = 0;
    virtual void visit(  UnaryExpression *e) = 0;
    virtual void visit(    NumberLiteral *e) = 0;
    virtual void visit(VariableReference *e) = 0;

    virtual void visit(ExpressionStatement *s) = 0;
    virtual void visit(   VarDeclStatement *s) = 0;
private:
};

class BinaryExpression : public BaseExpression {
public:
    CLASP_UTIL_CLASS_PTR(BinaryExpression)

    BinaryExpression(BaseExpression::Ptr left, clasp::lexical::Token::Ptr op, BaseExpression::Ptr right, clasp::type::Type::Ptr type = nullptr) : left_(left), op_(op), right_(right), BaseExpression(type) {}

    virtual void accept(Visitor *v) override { v->visit(this); }

    BaseExpression::Ptr left() const { return left_; }
    BaseExpression::Ptr right() const { return right_; }
    clasp::lexical::Token::Ptr op() const { return op_; }
private:
    BaseExpression::Ptr left_;
    clasp::lexical::Token::Ptr op_;
    BaseExpression::Ptr right_;
};

class UnaryExpression : public BaseExpression {
public:
    CLASP_UTIL_CLASS_PTR(UnaryExpression)

    UnaryExpression(clasp::lexical::Token::Ptr op, BaseExpression::Ptr right, clasp::type::Type::Ptr type = nullptr) : op_(op), right_(right), BaseExpression(type) {}

    virtual void accept(Visitor *v) override { v->visit(this); }

    clasp::lexical::Token::Ptr op() const { return op_; }
    BaseExpression::Ptr right() const { return right_; }
private:
    clasp::lexical::Token::Ptr op_;
    BaseExpression::Ptr right_;
};

class NumberLiteral : public BaseExpression {
public:
    CLASP_UTIL_CLASS_PTR(NumberLiteral)

    NumberLiteral(clasp::lexical::Token::Ptr num, clasp::type::Type::Ptr type = nullptr) : num_(num), BaseExpression(type) {}

    virtual void accept(Visitor *v) override { v->visit(this); }

    clasp::lexical::Token::Ptr num() const { return num_; }
private:
    clasp::lexical::Token::Ptr num_;
};

class VariableReference : public BaseExpression {
public:
    CLASP_UTIL_CLASS_PTR(VariableReference)

    VariableReference(clasp::lexical::Token::Ptr var, clasp::type::Type::Ptr type = nullptr) : var_(var), BaseExpression(type) {}

    virtual void accept(Visitor *v) override { v->visit(this); }

    clasp::lexical::Token::Ptr var() const { return var_; }
private:
    clasp::lexical::Token::Ptr var_;
};

class ExpressionStatement : public BaseStatement {
public:
    CLASP_UTIL_CLASS_PTR(ExpressionStatement)

    ExpressionStatement(BaseExpression::Ptr expr) : expr_(expr) {}

    virtual void accept(Visitor *v) override { v->visit(this); }

    BaseExpression::Ptr expr() const { return expr_; }
private:
    BaseExpression::Ptr expr_;
};

class VarDeclStatement : public BaseStatement {
public:
    CLASP_UTIL_CLASS_PTR(VarDeclStatement)
    enum class Qualifiers {
        MUTABLE, IMMUABLE, CONST
    };
    
    VarDeclStatement(std::string name, Qualifiers qual, clasp::type::Type::Ptr type, BaseExpression::Ptr initializer = nullptr) : name_(name), qual_(qual), type_(type), initializer_(initializer) {}

    virtual void accept(Visitor *v) override { v->visit(this); }

    Qualifiers qual() const { return qual_; }
    std::string name() const { return name_; }
    BaseExpression::Ptr initializer() const { return initializer_; }
    clasp::type::Type::Ptr type() const { return type_; }
private:
    Qualifiers qual_;
    std::string name_;
    BaseExpression::Ptr initializer_;
    clasp::type::Type::Ptr type_;
};
}

namespace clasp::util {

class ASTStringifier : public clasp::ast::Visitor {
public:
    using clasp::ast::Visitor::visit;

    std::string stringify(clasp::ast::BaseExpression::Ptr e);
    std::string stringify(clasp::ast::BaseStatement::Ptr e);

    virtual void visit( clasp::ast::BinaryExpression *e) override;
    virtual void visit(  clasp::ast::UnaryExpression *e) override;
    virtual void visit(    clasp::ast::NumberLiteral *e) override;
    virtual void visit(clasp::ast::VariableReference *e) override;

    virtual void visit(clasp::ast::ExpressionStatement *s) override;
    virtual void visit(   clasp::ast::VarDeclStatement *s) override;
private:
    std::string out_;
};
}
