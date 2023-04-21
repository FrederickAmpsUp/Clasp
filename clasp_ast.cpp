#ifndef CLASP_AST_CPP
#define CLASP_AST_CPP

#include <string>
#include <vector>

using namespace std;
enum Operation {
    ADD, SUB, 
    MUL, DIV,

    MOD, 
    AND, OR, XOR,

    NEG, NOT
};

class NotImplementedException {

};
class ASTVisitor;
// START EXPRESSION
class Expression {
    public:
        virtual void accept(ASTVisitor *visitor) = 0;
        virtual Expression *left()  = 0;
        virtual Expression *right() = 0;
        virtual string op()         = 0;

        virtual int value()         = 0;
        virtual string constant()   = 0;
};

class BinaryExpression : public Expression {
    Expression *l, *r;
    string operand;
    public:
        void accept(ASTVisitor *visitor) {
            visitor->visitBinaryExpression(this);
        }
        Expression *left()  {return l;      };
        Expression *right() {return r;      };
        string op()         {return operand;};

        int value() {throw NotImplementedException();};
        string constant() {throw NotImplementedException();};

        BinaryExpression (Expression *left, string op, Expression *right) {
            l = left;
            r = right;
            operand = op;
        };
};

class UnaryExpression : public Expression {
    Expression *r;
    string operand;
    public:
        void accept(ASTVisitor *visitor) {
            visitor->visitUnaryExpression(this);
        }
        Expression *right() {return r;};
        string  op() {return operand;};

        int value() {throw NotImplementedException();};
        string constant() {throw NotImplementedException();};
        Expression *left() {throw NotImplementedException();};

        UnaryExpression(string op, Expression *right) {
            operand = op;
            r = right;
        }

};

class Variable : public Expression {
    // TODO: implement this
};

class IntegerConstant : public Expression {
    int val;
    public:
        void accept(ASTVisitor *visitor) {
            visitor->visitIntegerConstant(this);
        }
        Expression *left() {throw NotImplementedException();};
        Expression *right() {throw NotImplementedException();};
        string op() {throw NotImplementedException();};

        int value() {return val;};
        string constant() {throw NotImplementedException();};

        IntegerConstant (int value) : val(value) {};
};

class FixedConstant : public Expression {
    int val;
    public:
        void accept(ASTVisitor *visitor) {
            visitor->visitFixedConstant(this);
        }
        Expression *left() {throw NotImplementedException();};
        Expression *right() {throw NotImplementedException();};
        string op() {throw NotImplementedException();};

        int value() {return val;};
        string constant() {throw NotImplementedException();};

        FixedConstant (int value) : val(value) {};
};

class StringConstant : public Expression {
    string val;
    public:
        void accept(ASTVisitor *visitor) {
            visitor->visitStringConstant(this);
        }
        Expression *left() {throw NotImplementedException();};
        Expression *right() {throw NotImplementedException();};
        string op() {throw NotImplementedException();};
        int value() {throw NotImplementedException();};

        string constant() {return val;};

        StringConstant (string value) {
            val = value;
        }

};
// END EXPRESSION

// START AST

class Line {
    public:
        virtual void accept (ASTVisitor *visitor) = 0;
};

class CodeBlock : public Line {
    public:
        void accept (ASTVisitor *visitor) override {
            visitor->visitCodeBlock (this);
        }
        vector<Line *> body;
        CodeBlock (vector<Line *> body) {
            this->body = body;
        }
};

class VariableDecl : public Line {
    public:
        void accept (ASTVisitor *visitor) override {
            visitor->visitVariableDecl(this);
        }

        string name;
        string type;

        Expression *initialiser;

        VariableDecl(string name, string type) {
            this->name = name;
            this->type = type;
            initialiser = nullptr;
        }

        VariableDecl(string name, string type, Expression *initialiser) {
            this->name = name;
            this->type = type;
            this->initialiser = initialiser;
        }
};

class Assignment : public Line {
    public:
        void accept (ASTVisitor *visitor) override {
            visitor->visitAssignment(this);
        }

        string name;
        Expression *value;

        Assignment(string name, Expression *value) {
            this->name = name;
            this->value = value;
        }
};

class FunctionCall : public Line {
    public:
        void accept (ASTVisitor *visitor) override {
            visitor->visitFunctionCall(this);
        }

        string name;
        vector<Expression *> args;

        FunctionCall(string name, vector<Expression *> args) {
            this->name = name;
            this->args = args;
        }
};

class FunctionDecl : public Line {
    public:
        void accept (ASTVisitor *visitor) override {
            visitor->visitFunctionDecl(this);
        }
        string name;
        CodeBlock *body;
        vector<VariableDecl *> args;
        string returnType;

        FunctionDecl (
            string name,
            CodeBlock *body,
            vector<VariableDecl *> arguments,
            string returnType
        ) {
            this->name = name;
            this->body = body;
            this->args = arguments;
            this->returnType = returnType;
        }
};

// END AST

class ASTVisitor {
    public:
        virtual void visit                             (Line *node) = 0;
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