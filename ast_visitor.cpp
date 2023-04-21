#ifndef AST_VISITOR_CPP
#define AST_VISITOR_CPP

class Statement;
class VariableDecl;
class Assignment;
class FunctionCall;
class FunctionDecl;
class CodeBlock;

class Expression;
class BinaryExpression;
class UnaryExpression;
class IntegerConstant;
class FixedConstant;
class StringConstant;
#include "clasp_ast.cpp"

class ASTVisitor {
    public:
        virtual void visit                             (Statement *node) = 0;
        virtual void visitVariableDecl         (VariableDecl *node) = 0;
        virtual void visitAssignment             (Assignment *node) = 0;
        virtual void visitFunctionCall         (FunctionCall *node) = 0;
        virtual void visitFunctionDecl         (FunctionDecl *node) = 0;
        virtual void visitCodeBlock               (CodeBlock *node) = 0;

        virtual void visitExpression             (Expression *node) = 0;
        virtual void visitBinaryExpression (BinaryExpression *node) = 0;
        virtual void visitUnaryExpression   (UnaryExpression *node) = 0;
        virtual void visitIntegerConstant   (IntegerConstant *node) = 0;
        virtual void visitFixedConstant       (FixedConstant *node) = 0;
        virtual void visitStringConstant     (StringConstant *node) = 0;
};
#endif