#ifndef CCLASP_CPP
#define CCLASP_CPP

#ifndef MAIN
#define MAIN

#include "parser.cpp"
#include "lexer.cpp"
#include "clasp_ast.cpp"
#include "ast_visitor.cpp"
#include "get_type.cpp"
#include <string>
#include <map>

using namespace std;
class Interpreter : public ASTVisitor {
    int memory[1000];
    int numvars = 0;
    map<string, int> variables;
    map<string, CodeBlock*> functions;
    
public:
    Expression *visitExpression(Expression* node) {
        return node->accept(this);
    }
    Expression *visitBinaryExpression(BinaryExpression* node) {
        Expression *lval = visitExpression(node->left());
        Expression *rval = visitExpression(node->right());
        if (lval->is_stringConstant() || rval->is_stringConstant()) error("UnsupportedFeatureError", "String constants are not yet supported");

        int left;
        if (lval->is_numberConstant())
            left = lval->value();
        else
            left = variables[lval->constant()];
        int right;
        if (rval->is_numberConstant())
            right = rval->value();
        else
            right = variables[rval->constant()];

        string op = node->op();
        if (op == "+") {
            return new IntegerConstant(left + right);
        } else if (op == "-") {
            return new IntegerConstant(left - right);
        } else if (op == "*") {
            return new IntegerConstant(left * right);
        } else if (op == "/") {
            return new IntegerConstant(left / right);
        }
        error("UnsupportedFeatureError", "Unsupported operator: " + op);
    }
    Expression *visitUnaryExpression(UnaryExpression* node) {
        Expression *rval = visitExpression(node->right());

        int right;
        if (rval->is_numberConstant())
            right = rval->value();
        else
            right = variables[rval->constant()];
        
        string op = node->op();
        if (op == "-") {
            return new IntegerConstant(-right);
        }
        error("UnsupportedFeatureError", "Unsupported operator: " + op);
    }

    Expression *visitIntegerConstant(IntegerConstant* node) {
        return node;
    }

    Expression *visitFixedConstant(FixedConstant* node) {
        return node;
    }

    Expression *visitStringConstant(StringConstant* node) {
        error("UnsupportedFeatureError", "String constants are not yet supported");
    }

    Expression *visitVariable(Variable* node) {
        return new IntegerConstant(memory[variables[node->constant()]]);
    }

    void visit(Statement *node) {
        node->accept(this);
    }
    void visitVariableDecl(VariableDecl *node) {
        variables[node->name] = numvars;
        if (node->initialiser != nullptr) {
            memory[numvars] = this->visitExpression(node->initialiser)->value();
        }
        numvars++;
    }
    void visitAssignment(Assignment* node) {
        memory[variables[node->name]] = this->visitExpression(node->value)->value();
    }
    void visitFunctionCall(FunctionCall* node) {
        if (node->name == "") return;
        if (node->name == "print") {
            for (Expression *arg : node->args) {
                std::cout << visitExpression(arg)->value() << std::endl;
            }
        } else {
            visitCodeBlock(functions[node->name]);
        }
    }
    void visitFunctionDecl(FunctionDecl *node) {
        functions[node->name] = node->body;
    }
    void visitCodeBlock(CodeBlock* node) {
        for (Statement *statement : node->body) {
            visit((Statement *)statement);
        }
    }
    void visitWhile(While* node) {
        while (visitExpression(node->cond)->value() != 0) {
            visitCodeBlock(node->body);
        }
    }
    void visitIf(If* node) {
        if(visitExpression(node->cond)->value() != 0) {
            visitCodeBlock(node->body);
        }
    }
};

int main (int argc, char* argv[]) {
    string code = R"( {
var n1: int = 0;
var n2: int = 1;

while(true) {
    print(n1);
    var tmp: int = n1 + n2;
    n1 = n2;
    n2 = tmp;
}
} )";
    vector<Token> tokens;
    tokens = parse_tokens(code);
    ASTParser parser {tokens};
    Interpreter interpreter;
    interpreter.visit(parser.codeblock());
}

#endif
#endif