#ifndef CLASP_AST_CPP
#define CLASP_AST_CPP

#include <string>
#include <vector>
#include <iostream>

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

//class ASTVisitor;
#include "ast_visitor.cpp"

// START EXPRESSION
class Expression {
    public:
        virtual bool is_numberConstant() = 0;
        virtual bool is_stringConstant() = 0;
        virtual Expression *accept(ASTVisitor *visitor) = 0;
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
        bool is_numberConstant() {return false;}
        bool is_stringConstant() {return false;}
        Expression *accept(ASTVisitor *visitor) {
            return visitor->visitBinaryExpression(this);
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
        bool is_numberConstant() {return false;}
        bool is_stringConstant() {return false;}
        Expression *accept(ASTVisitor *visitor) {
            return visitor->visitUnaryExpression(this);
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

class IntegerConstant : public Expression {
    int val;
    public:
        bool is_stringConstant() {return false;}
        bool is_numberConstant() {return true;}
        Expression *accept(ASTVisitor *visitor) {
            return visitor->visitIntegerConstant(this);
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
        bool is_stringConstant() {return false;}
        bool is_numberConstant() {return true;}
        Expression *accept(ASTVisitor *visitor) {
            return visitor->visitFixedConstant(this);
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
        bool is_numberConstant() {return false;}
        bool is_stringConstant() {return true;}
        Expression *accept(ASTVisitor *visitor) {
            return visitor->visitStringConstant(this);
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

class Variable : public Expression {
    string val;
    public:
        bool is_numberConstant() {return false;}
        bool is_stringConstant() {return false;}
        Expression *accept(ASTVisitor *visitor) {
            return visitor->visitVariable(this);
        }
        Expression *left()  {throw NotImplementedException();};
        Expression *right() {throw NotImplementedException();};
        string op() {throw NotImplementedException();};
        int value() {throw NotImplementedException();};

        string constant() {return val;};

        Variable (string name) {
            val = name;
        }
};

class ListConstant : public Expression {
public:
    vector<Expression *> values;

    bool is_numberConstant() {return false;}
    bool is_stringConstant() {return false;}
    Expression *accept(ASTVisitor *visitor) {
        return visitor->visitListConstant(this);
    }
    Expression *left()  {throw NotImplementedException();};
    Expression *right() {throw NotImplementedException();};
    string op() {throw NotImplementedException();};
    int value() {throw NotImplementedException();};

    string constant() {throw NotImplementedException();};
};

class FunctionValue : public Expression {
public:
    string name;
    vector<Expression *> args;
    Expression *accept(ASTVisitor *visitor) {
        return visitor->visitFunctionValue(this);
    }
    bool is_numberConstant() {return false;}
    bool is_stringConstant() {return false;}
    
    Expression *left()  {throw NotImplementedException();};
    Expression *right() {throw NotImplementedException();};
    string op() {throw NotImplementedException();};
    int value() {throw NotImplementedException();};
    string constant() {throw NotImplementedException();};
    
    FunctionValue(string name, vector<Expression *> args) {
        this->name = name;
        this->args = args;
    }
};

// END EXPRESSION

// START AST

class Statement {
    public:
        virtual void accept (ASTVisitor *visitor) = 0;
};

class CodeBlock : public Statement {
    public:
        void accept (ASTVisitor *visitor) {
            visitor->visitCodeBlock (this);
        }
        vector<Statement *> body;
        CodeBlock (vector<Statement *> body) {
            this->body = body;
        }
};

class VariableDecl : public Statement {
    public:
        void accept (ASTVisitor *visitor) {
            visitor->visitVariableDecl (this);
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

class Assignment : public Statement {
    public:
        void accept (ASTVisitor *visitor) {
            visitor->visitAssignment(this);
        }

        string name;
        Expression *value;

        Assignment(string name, Expression *value) {
            this->name = name;
            this->value = value;
        }
};

class FunctionCall : public Statement {
    public:
        void accept (ASTVisitor *visitor) {
            visitor->visitFunctionCall(this);
        }

        string name;
        vector<Expression *> args;

        FunctionCall(string name, vector<Expression *> args) {
            this->name = name;
            this->args = args;
        }
};

class FunctionDecl : public Statement {
    public:
        void accept (ASTVisitor *visitor) {
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

class While : public Statement {
    public:
        void accept(ASTVisitor *visitor) {
            visitor->visitWhile(this);
        }
        CodeBlock *body;
        Expression *cond;

        While (CodeBlock *body, Expression *cond) {
            this->body = body;
            this->cond = cond;
        }
};

class If : public Statement {
    public:
        void accept(ASTVisitor *visitor) {
            visitor->visitIf(this);
        }

        CodeBlock *body;
        Expression *cond;

        If (CodeBlock *body, Expression *cond) {
            this->body = body;
            this->cond = cond;
        }
};

class Return : public Statement {
public:
        void accept(ASTVisitor *visitor) {
            visitor->visitReturn(this);
        }
        Expression *value;
        
        Return(Expression *value) {
            this->value = value;
        }
};

// END AST

#endif