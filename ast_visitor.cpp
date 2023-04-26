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
class Variable;
class While;
class If;

#include "clasp_ast.cpp"

class ASTVisitor {
    public:
        virtual void visit                        (Statement *node) = 0;
        virtual void visitVariableDecl         (VariableDecl *node) = 0;
        virtual void visitAssignment             (Assignment *node) = 0;
        virtual void visitFunctionCall         (FunctionCall *node) = 0;
        virtual void visitFunctionDecl         (FunctionDecl *node) = 0;
        virtual void visitCodeBlock               (CodeBlock *node) = 0;
        virtual void visitWhile                       (While *node) = 0;
        virtual void visitIf                             (If *node) = 0;

        virtual Expression *visitExpression             (Expression *node) = 0;
        virtual Expression *visitBinaryExpression (BinaryExpression *node) = 0;
        virtual Expression *visitUnaryExpression   (UnaryExpression *node) = 0;
        virtual int        *visitIntegerConstant   (IntegerConstant *node) = 0;
        virtual int        *visitFixedConstant       (FixedConstant *node) = 0;
        virtual string     *visitStringConstant     (StringConstant *node) = 0;
        virtual Expression *visitVariable                 (Variable *node) = 0;
};
#endif