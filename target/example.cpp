#include <clasp/target.hpp>
#include <memory>

class ExampleTarget : public clasp::target::ASTVisitorTarget {
public:
    CLASP_UTIL_CLASS_PTR(ExampleTarget)
    using clasp::target::ASTVisitorTarget::visit;

    virtual void visit( clasp::ast::BinaryExpression *e) override {
        this->out_ += "(b ";
        this->visit(e->left());
        this->out_ += " " + e->op()->value + " ";
        this->visit(e->right());
        this->out_ += ")";
    }
    virtual void visit(  clasp::ast::UnaryExpression *e) override {
        this->out_ += "(u " + e->op()->value;
        this->visit(e->right());
        this->out_ += ")";
    }
    virtual void visit(    clasp::ast::NumberLiteral *e) override {
        this->out_ += "(number " + e->num()->value + ")";
    }
    virtual void visit(clasp::ast::VariableReference *e) override {
        this->out_ += "(v " + e->var()->value + ")";
    }

    virtual void visit(clasp::ast::ExpressionStatement *s) override {
        this->out_ += "(expr ";
        this->visit(s->expr());
        this->out_ += ")\n";
    }

    virtual std::vector<uint8_t> output() override {
        return std::vector<uint8_t>(out_.begin(), out_.end());
    }
private:
    std::string out_;
};

extern "C"
ExampleTarget::Ptr instantiate() {
    return ExampleTarget::make_ptr();
}