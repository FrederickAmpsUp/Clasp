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
#include <fstream>
#include <iostream>

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
        if (op == "+") { // surely there's a better way to do this ... ?
            return new IntegerConstant(left + right);
        } else if (op == "-") {
            return new IntegerConstant(left - right);
        } else if (op == "*") {
            return new IntegerConstant(left * right);
        } else if (op == "/") {
            return new IntegerConstant(left / right);
        } else if (op == "==") {
            return new IntegerConstant(left == right);
        } else if (op == "!=") {
            return new IntegerConstant(left != right);
        } else if (op == ">") {
            return new IntegerConstant(left > right);
        } else if (op == "<") {
            return new IntegerConstant(left < right);
        } else if (op == ">=") {
            return new IntegerConstant(left >= right);
        } else if (op == "<=") {
            return new IntegerConstant(left <= right);
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

string load_file(const char* fname) {
    ifstream file(fname);
    if (!file.is_open()) {
        error("Could not open file", (string)fname);
    }
    string file_contents((istreambuf_iterator<char>(file)),
                              istreambuf_iterator<char>());
    file.close();
    return file_contents;
}

int main (int argc, char* argv[]) {
    if (argc < 2) error("usage", "cclasp <filename>");
    string code = "{";
    code += load_file(argv[1]);
    code += '}';
    vector<Token> tokens;
    tokens = parse_tokens(code);
    ASTParser parser {tokens};
    Interpreter interpreter;
    interpreter.visit(parser.codeblock());
}

#endif
#endif