#pragma once

#include <variant>
#include <memory>

namespace Clasp::AST {
class Node {};

namespace Expression {
class Expression {};
class Binary : public Expression {
public:
    Binary(std::shared_ptr<Expression> left, std::shared_ptr<Expression> right);
private:
    std::shared_ptr<Expression> left, right;
};
};
};