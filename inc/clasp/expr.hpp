#ifndef _INC_CLASP_EXPR_HPP
#define _INC_CLASP_EXPR_HPP

#include <memory>
#include <clasp/lexer.hpp>
#include <clasp/int_rep.hpp>

namespace clasp::expr {
class BaseExpr {
public:
    using Ptr = std::shared_ptr<BaseExpr>;

    virtual clasp::intermediate::IRProgram::Ptr compile() = 0;
private:

};

class Binary : public BaseExpr {
public:
    virtual clasp::intermediate::IRProgram::Ptr compile() override;
private:
    BaseExpr::Ptr left, right;
    clasp::lexical::Token::Ptr op;
};
};

#endif /* _INC_CLASP_EXPR_HPP */