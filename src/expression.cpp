#include <clasp/ast.hpp>

namespace Clasp::AST::Expression {

Binary::Binary(std::shared_ptr<Expression> left, std::shared_ptr<Expression> right) {
    this->left = left;
    this->right = right;
}
};