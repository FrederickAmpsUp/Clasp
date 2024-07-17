#include <clasp/ast.hpp>

namespace clasp::util {

std::string ASTStringifier::stringify(clasp::ast::BaseExpression::Ptr e) {
    this->out_.clear();
    this->visit(e);
    return this->out_;
}
std::string ASTStringifier::stringify(clasp::ast::BaseStatement::Ptr s) {
    this->out_.clear();
    this->visit(s);
    return this->out_;
}

void ASTStringifier::visit(clasp::ast::BinaryExpression *e) {
    this->out_ += "(b ";
    this->visit(e->left());
    this->out_ += " " + e->op()->value + " ";
    this->visit(e->right());
    this->out_ += ")";
}

void ASTStringifier::visit(clasp::ast::UnaryExpression *e) {
    this->out_ += "(u " + e->op()->value;
    this->visit(e->right());
    this->out_ += ")";
}

void ASTStringifier::visit(clasp::ast::NumberLiteral *e) {
    this->out_ += "(number " + e->num()->value + ")";
}

void ASTStringifier::visit(clasp::ast::VariableReference *e) {
    this->out_ += "(v " + e->var()->value + ")";
}

void ASTStringifier::visit(clasp::ast::ExpressionStatement *s) {
    this->out_ += "(expr ";
    this->visit(s->expr());
    this->out_ += ")\n";
}
}