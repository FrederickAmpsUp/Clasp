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

class Expression;
class BinaryExpression;
class UnaryExpression;
class IntegerConstant;
class FixedConstant;
class StringConstant;
class Variable;
class FunctionValue;

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
        virtual Expression *visitIntegerConstant   (IntegerConstant *node) = 0;
        virtual Expression *visitFixedConstant       (FixedConstant *node) = 0;
        virtual Expression *visitStringConstant     (StringConstant *node) = 0;
        virtual Expression *visitVariable                 (Variable *node) = 0;
        virtual Expression *visitFunctionValue       (FunctionValue *node) = 0;
};

/*
class ASTPrinter : public ASTVisitor {
public:
    void visit(Statement *node) {
        node->accept(this);
    }

    void visitVariableDecl(VariableDecl *node) {
        std::cout << "VariableDecl (name=\"" << node->name << "\" type=\"" << node->type << "\" initializer=";
        if (node->initialiser != nullptr) {
            visitExpression(node->initialiser);
        }
        cout << ")";
    }

    void visitAssignment(Assignment *node) {
        std::cout << "Assignment (name=\"" << node->name << "\" value=";
        visitExpression(node->value);
        std::cout << ")";
    }

    void visitFunctionCall(FunctionCall *node) {
        if (node->name == "") return;
        std::cout << "FunctionCall (name=\"" << node->name << "\" arguments=[";
        for (Expression *arg : node->args) {
            this->visitExpression(arg);
            std::cout << ",";
        }
        std::cout << "])";
    }

    void visitFunctionDecl(FunctionDecl *node) {
        std::cout << "FunctionDecl (name=\"" << node->name << "\" rettype=\"" << node->returnType << "\" arguments=[";
        for (VariableDecl *arg : node->args) {
            this->visit(arg);
            std::cout << ",";
        }
        std::cout << "] body=";
        this->visit(node->body);
        std::cout << ")";
    }

    void visitCodeBlock(CodeBlock *node) {
        std::cout << "CodeBlock (body=[" << std::endl;
        for (Statement *line : node->body) {
            this->visit(line);
            std::cout << std::endl;
        }
        std::cout << "])";
    }

    Expression *visitExpression(Expression *node) {
        node->accept(this);
        return nullptr;
    }

    Expression *visitBinaryExpression(BinaryExpression *node) {
        std::cout << "BinaryExpression (left=";
        this->visitExpression(node->left());
        std::cout << " op=" << node->op() << " right=";
        this->visitExpression(node->right());
        std::cout << ")";
    }

    Expression *visitUnaryExpression(UnaryExpression *node) {
        std::cout << "UnaryExpression (op=" << node->op() << " right=";
        this->visitExpression(node->right());
        std::cout << ")";
    }

    Expression *visitIntegerConstant(IntegerConstant *node) {
        std::cout << "IntegerConstant (value=" << node->value() << ")";
    }

    Expression *visitFixedConstant(FixedConstant *node) {
        std::cout << "FixedConstant (value=" << (node->value() / 65536.0) << ")";
    }

    Expression *visitStringConstant(StringConstant *node) {
        std::cout << "StringConstant (value=\"" << node->constant() << "\")";
    }

    Expression *visitVariable(Variable *node) {
        std::cout << "Variable (name=" << node->constant() << ")";
    }

    void visitWhile(While *node) {
        std::cout << "While (cond=";
        this->visitExpression(node->cond);
        std::cout << " body=";
        this->visitCodeBlock(node->body);
        std::cout << ")";
    }

    void visitIf(If *node) {
        std::cout << "If (cond=";
        this->visitExpression(node->cond);
        std::cout << " body=";
        this->visitCodeBlock(node->body);
        std::cout << ")";
    }
};*/

#endif