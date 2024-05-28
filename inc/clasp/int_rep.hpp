#ifndef _INC_CLASP_INT_REP_HPP
#define _INC_CLASP_INT_REP_HPP

#include <vector>
#include <memory>
#include <optional>
#include <initializer_list>

namespace clasp::intermediate {

struct IRInstruction {
    enum OpcodeType {
        PUSH, POP,
        ADD, SUB,
    } opcode;

    IRInstruction(OpcodeType opcode, std::vector<uint64_t> arguments={}) { this->opcode = opcode; this->arguments = arguments; }

    std::vector<uint64_t> arguments;

    using Ptr = std::shared_ptr<IRInstruction>;
    static Ptr New(OpcodeType opcode, std::vector<uint64_t> arguments={}) { return std::make_shared<IRInstruction>(opcode, arguments); }
};

class IRProgram {
public:
    using Ptr = std::shared_ptr<IRProgram>;
    static Ptr New() { return std::make_shared<IRProgram>(); }

    void add(IRInstruction::Ptr instruction);
    void add(std::initializer_list<IRInstruction::Ptr> instructions);

    void add(IRProgram::Ptr prog);

    std::vector<IRInstruction::Ptr> getInstructions() const;

    auto begin() const { return instructions.begin(); }
    auto end() const { return instructions.end(); }

    IRProgram::Ptr optimize() const;
private:
    std::vector<IRInstruction::Ptr> instructions;
};

class IRCompiler {

};

};

#endif /* _INC_CLASP_INT_REP_HPP */