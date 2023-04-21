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

// START EXPRESSION
class Expression {
    public:
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

};

class CodeBlock {
    public:
        vector<Line *> body;
        CodeBlock (vector<Line *> body) {
            this->body = body;
        }
};

class VariableDecl : Line {
    public:
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
        string name;
        Expression *value;

        Assignment(string name, Expression *value) {
            this->name = name;
            this->value = value;
        }
};

class FunctionCall : public Line {
    public:
        string name;
        vector<Expression *> args;

        FunctionCall(string name, vector<Expression *> args) {
            this->name = name;
            this->args = args;
        }
};

class FunctionDecl : public Line {
    public:
        string name;
        CodeBlock *body;
        vector<VariableDecl> arguments;
        string returnType;

        FunctionDecl (
            string name,
            CodeBlock *body,
            vector<VariableDecl> arguments,
            string returnType
        ) {
            this->name = name;
            this->body = body;
            this->arguments = arguments;
            this->returnType = returnType;
        }
};

// END AST

#endif