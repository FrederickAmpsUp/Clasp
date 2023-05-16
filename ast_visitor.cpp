#ifndef AST_VISITOR_CPP
#define AST_VISITOR_CPP

class Statement;
class VariableDecl;
class Assignment;
class FunctionCall;
class FunctionDecl;
class CodeBlock;
class While;
class If;
class Return;

class Expression;
class BinaryExpression;
class UnaryExpression;
class IntegerConstant;
class FixedConstant;
class StringConstant;
class Variable;
class FunctionValue;
class ListConstant;

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
        virtual void visitReturn                     (Return *node) = 0;

        virtual Expression *visitExpression             (Expression *node) = 0;
        virtual Expression *visitBinaryExpression (BinaryExpression *node) = 0;
        virtual Expression *visitUnaryExpression   (UnaryExpression *node) = 0;
        virtual Expression *visitIntegerConstant   (IntegerConstant *node) = 0;
        virtual Expression *visitFixedConstant       (FixedConstant *node) = 0;
        virtual Expression *visitStringConstant     (StringConstant *node) = 0;
        virtual Expression *visitVariable                 (Variable *node) = 0;
        virtual Expression *visitFunctionValue       (FunctionValue *node) = 0;
        virtual Expression *visitListConstant         (ListConstant *node) = 0;
};

#endif