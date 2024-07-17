#pragma once

#include <string>
#include <memory>
#include <clasp/util.hpp>
#include <clasp/lexical.hpp>

namespace clasp::ast {

class Visitor;

class BaseExpression {
public:
    CLASP_UTIL_CLASS_PTR(BaseExpression)

    virtual void accept(Visitor *v) = 0;
private:
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

class Visitor {
public:
    void visit(BaseExpression::Ptr e) { e->accept(this); }
    void visit(BaseStatement::Ptr e) { e->accept(this); }

    virtual void visit( BinaryExpression *e) = 0;
    virtual void visit(  UnaryExpression *e) = 0;
    virtual void visit(    NumberLiteral *e) = 0;
    virtual void visit(VariableReference *e) = 0;

    virtual void visit(ExpressionStatement *e) = 0;
private:
};

class BinaryExpression : public BaseExpression {
public:
    CLASP_UTIL_CLASS_PTR(BinaryExpression)

    BinaryExpression(BaseExpression::Ptr left, clasp::lexical::Token::Ptr op, BaseExpression::Ptr right) : left_(left), op_(op), right_(right) {}

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

    UnaryExpression(clasp::lexical::Token::Ptr op, BaseExpression::Ptr right) : op_(op), right_(right) {}

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

    NumberLiteral(clasp::lexical::Token::Ptr num) : num_(num) {}

    virtual void accept(Visitor *v) override { v->visit(this); }

    clasp::lexical::Token::Ptr num() const { return num_; }
private:
    clasp::lexical::Token::Ptr num_;
};

class VariableReference : public BaseExpression {
public:
    CLASP_UTIL_CLASS_PTR(VariableReference)

    VariableReference(clasp::lexical::Token::Ptr var) : var_(var) {}

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

    virtual void visit(clasp::ast::ExpressionStatement *e) override;
private:
    std::string out_;
};
}
