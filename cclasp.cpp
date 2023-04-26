#ifndef CCLASP_CPP
#define CCLASP_CPP

#ifndef MAIN
#define MAIN

#include "parser.cpp"
#include "lexer.cpp"
#include "clasp_ast.cpp"
#include "ast_visitor.cpp"
#include <string>
#include <map>

using namespace std;
class Interpreter : public ASTVisitor {
    int memory[1000];
    int numvars = 0;
    map<string, int> variables;
    
public:
    Expression *visitExpression(Expression* node) {
        return node->accept(this);
    }
    Expression *visitBinaryExpression(BinaryExpression* node) {
        Expression *lval = visitExpression(node->left());
        Expression *rval = visitExpression(node->right());
    }

    void visit(Statement *node) {
        node->accept(this);
    }
    void visitVariableDecl(VariableDecl *node) {
        variables[node->name] = numvars;
        if (node->initialiser != nullptr) {

        }
        numvars++;
    }
};

int main (int argc, char* argv[]) {

}

#endif
#endif