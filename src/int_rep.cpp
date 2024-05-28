#include <clasp/int_rep.hpp>

namespace clasp::intermediate {

void IRProgram::add(IRInstruction::Ptr instruction) {
    if (!instruction) return;
    this->instructions.push_back(instruction);
}

void IRProgram::add(std::initializer_list<IRInstruction::Ptr> instructions) {
    for (const auto& instruction : instructions) {
        this->add(instruction);
    }
}

void IRProgram::add(IRProgram::Ptr program) {
    if (!program) return;
    for (const auto& instruction : *program) {
        this->add(instruction);
    }
}

std::vector<IRInstruction::Ptr> IRProgram::getInstructions() const {
    return this->instructions;
}

IRProgram::Ptr IRProgram::optimize() const {
        // copy and return
    return std::make_shared<IRProgram>(*this);

    // TODO: run through the code and optimize it
}
};