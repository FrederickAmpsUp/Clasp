#include "clasp_ast.cpp"
#include <iostream>
#include <typeinfo>

using namespace std;

class NodePrinter : NodeVisitor {
    void visit(ASTNode *node) {
        std:: cout << typeid(*node).name() << std::endl;
    }
};

int main () {
    Expression *expression = new BinaryExpression(
        new BinaryExpression (
            new IntegerConstant(5),
            ADD,
            new IntegerConstant(6)
        ),
        MUL,
        new IntegerConstant(2)
    );

    std::cout << expression->left()->right()->value() << std::endl;
    
    return 0;
}