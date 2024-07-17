#pragma once

#include <clasp/ast.hpp>
#include <clasp/util.hpp>
#include <vector>

namespace clasp::target {

class ASTVisitorTarget : public clasp::ast::Visitor {
public:
    using clasp::ast::Visitor::visit;

    CLASP_UTIL_CLASS_PTR(ASTVisitorTarget)

    virtual std::vector<uint8_t> output() = 0;
};

class BuildTarget {
public:
    BuildTarget(const std::string& filename);

    enum class Type {
        AST_VISITOR,
        // BYTECODE_TRANSPILER
    };

    std::vector<uint8_t> run(clasp::ast::BaseStatement::Ptr stmt);
private:
    Type type;
    //union {
        ASTVisitorTarget::Ptr visitor_;
    //};
};


using VisitorTargetCreateFunc = ASTVisitorTarget::Ptr(*)();
}