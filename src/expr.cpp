#include <clasp/expr.hpp>
#include <clasp/log.hpp>

namespace clasp::expr {

using namespace clasp::intermediate;

IRProgram::Ptr Binary::compile() {
    IRProgram::Ptr program = IRProgram::New();
    program->add(this->left->compile());
    program->add(this->right->compile());

    switch (this->op->type) {
        case clasp::lexical::Token::Type::PLUS: {
            program->add(IRInstruction::New(IRInstruction::OpcodeType::ADD));
        } break;

        case clasp::lexical::Token::Type::MINUS: {
            program->add(IRInstruction::New(IRInstruction::OpcodeType::SUB));
        } break;

        default: {
                // TODO: better error messages
            throw clasp::SyntaxError(this->op->where, "Unknown binary operator '" + this->op->value);
        }
    }

    return program;
};
};