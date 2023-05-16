#ifndef CCLASP_CPP
#define CCLASP_CPP

//#define NOMAIN_CLASP_CPP

#include "parser.cpp"
#include "lexer.cpp"
#include "clasp_ast.cpp"
#include "ast_visitor.cpp"
#include "get_type.cpp"
#include <string>
#include <map>
#include <fstream>
#include <iostream>
#include <tuple>
#include <typeinfo>
#include <stdexcept>

using namespace std;
class Interpreter : public ASTVisitor {
    int memory[1000];
    int numvars = 0;
    int scope = 0;
    Expression *returned;
    map<string, tuple<int,int>> variables;
    map<string, FunctionDecl*> functions;
    
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
            left = get<0>(variables[lval->constant()]);
        int right;
        if (rval->is_numberConstant())
            right = rval->value();
        else
            right = get<0>(variables[rval->constant()]);
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
            right = get<0>(variables[rval->constant()]);
        
        string op = node->op();
        if (op == "-") {
            return new IntegerConstant(-right);
        }
        error("UnsupportedFeatureError", "Unsupported unary operator: " + op);
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
        return new IntegerConstant(memory[get<0>(variables[node->constant()])]);
    }
    
    Expression *visitFunctionValue(FunctionValue* node) {
        scope++;
        if (node->name == "ptr") {
            if (node->args[0]->is_numberConstant()) error("MemoryError", "Cannot take address of a number constant");
            if (node->args[0]->is_stringConstant()) error("MemoryError", "Cannot take address of a string constant");
            scope--;
            try {
                return new IntegerConstant(get<0>(variables[node->args[0]->constant()]));
            } catch (NotImplementedException e) {
                error("MemoryError", "Cannot take address of an expression");
            }
        } else {
            visitFunctionCall(new FunctionCall(
                node->name,
                node->args
            ));
            return returned;
        }
        error("FunctionUndefinedError", "Function \"" + node->name + "\" is not defined");
        scope--;
        variableScope();
    }
    
    Expression *visitListConstant(ListConstant* node) {
        error("uhh", "hmm");
    }
    
    void variableScope() {
        unsigned int highest_addr = 0;
        std::vector<string> toErase{};
        for (auto itr = variables.begin(); itr != variables.end(); ++itr) {
            //cout << ((itr != variables.end()) ? "true" : "false") << endl;
            if (itr->first == "") continue;
            if (get<0>(itr->second) > highest_addr) highest_addr = get<0>(itr->second);
            if (get<1>(itr->second) > scope) {
                toErase.push_back(itr->first);
            }
        }
        for (string name: toErase) {
            variables.erase(name);
        }
        numvars = highest_addr;
    }
    
    void visit(Statement *node) {
        //cout << typeid(*node).name() << endl;
        node->accept(this);
    }
    void visitVariableDecl(VariableDecl *node) {
        variables[node->name] = tuple<int, int> {numvars, scope};
        if (node->initialiser != nullptr) {
            memory[numvars] = this->visitExpression(node->initialiser)->value();
        }
        numvars++;
    }
    void visitAssignment(Assignment* node) {
        memory[get<0>(variables[node->name])] = this->visitExpression(node->value)->value();
    }
    void visitFunctionCall(FunctionCall* node) {
        if (node->name == "") return;
        
        if (node->name == "printChar") {
            for (Expression *arg : node->args) {
                std::cout << (char)visitExpression(arg)->value();
            }
        } else if (node->name == "print") {
            for (Expression *arg : node->args) {
                std::cout << visitExpression(arg)->value() << std::endl;
            }
        } else if (node->name == "write") {
            if (node->args.size() < 2) error("ArgumentError", "Not enough arguments for write() function");
            memory[visitExpression(node->args[0])->value()] = visitExpression(node->args[1])->value();
        } else if (node->name == "read") {
            if (node->args.size() < 1) error("ArgumentError", "Not enough arguments for read() function");
            returned = new IntegerConstant(memory[visitExpression(node->args[0])->value()]);
        } else {
            scope++;
            if (functions.count(node->name) == 0) error("FunctionNotDefinedError", "Function \"" + node->name + "\" not defined.");
            for (int i = 0; i < node->args.size(); i++) {
                visitVariableDecl(
                    new VariableDecl(functions[node->name]->args[i]->name, functions[node->name]->args[i]->type, node->args[i])
                );
                //cout << i << " " << functions[node->name]->args[i]->name << endl;
            }
            visitCodeBlock(functions[node->name]->body);
            scope--;
            variableScope();
        }
    }
    void visitFunctionDecl(FunctionDecl *node) {
        functions[node->name] = node;
    }
    void visitCodeBlock(CodeBlock* node) {
        for (Statement *statement : node->body) {
            visit(statement);
        }
    }
    void visitWhile(While* node) {
        while (visitExpression(node->cond)->value() != 0) {
            scope++;
            visitCodeBlock(node->body);
            scope--;
            variableScope();
        }
    }
    void visitIf(If* node) {
        if(visitExpression(node->cond)->value() != 0) {
            scope++;
            visitCodeBlock(node->body);
            scope--;
            variableScope();
        }
    }
    void visitReturn(Return* node) {
        returned = visitExpression(node->value);
    }
};

class SHARKCompiler : public ASTVisitor {
    int numvars = 0;
    int scope = 0;
    map<string, tuple<int,int>> variables;
    map<string, FunctionDecl*> functions;
    
public:
    string out;
    Expression *visitExpression(Expression* node) {
        return node->accept(this);
    }
    
    Expression *visitBinaryExpression(BinaryExpression* node) {
        visitExpression(node->left());
        visitExpression(node->right());
        out += "pl alu2 _ _\npl alu1 _ _\n";
        string op = node->op();
        if (op == "+") { // surely there's a better way to do this ... ? perhaps a map
            out += "add _ _ _\nph a _ _\n";
        } else if (op == "-") {
            out += "sub _ _ _\nph a _ _\n";
        } else if (op == "*") {
            out += "mul _ _ _\nph a _ _\n";
        } else if (op == "/") {
            out += "div _ _ _\nph a _ _\n";
        } else if (op == "==") {
            out += "eq _ _ _\nph a _ _\n";
        } else if (op == "!=") {
            out += "neq _ _ _\nph a _ _\n";
        } else if (op == ">") {
            out += "gt _ _ _\nph a _ _\n";
        } else if (op == "<") {
            out += "lt _ _ _\nph a _ _\n";
        } else if (op == ">=") {
            out += "ge _ _ _\nph a _ _\n";
        } else if (op == "<=") {
            out += "le _ _ _\nph a _ _\n";
        } else {
            error("UnsupportedFeatureError", "Unsupported operator: " + op);
        }
    }
    Expression *visitUnaryExpression(UnaryExpression* node) {
        error("UnsupportedFeatureError", "Unary Expressions are not supported in the SHARK architecture yet."); // WIP
    }

    Expression *visitIntegerConstant(IntegerConstant* node) {
        out += "imm a ";
        out += to_string(node->value());
        out += " _\nph a _ _\n";
    }

    Expression *visitFixedConstant(FixedConstant* node) {
        error("UnsupportedFeatureError", "Fixed-point numbers are not supported in the SHARK architecture yet."); // WIP
    }

    Expression *visitStringConstant(StringConstant* node) {
        error("UnsupportedFeatureError", "String constants are not supported in the SHARK architecture.");
    }

    Expression *visitVariable(Variable* node) {
        out += "ld a ";
        out += to_string(get<0>(variables[node->constant()]));
        out += " _\nph a _ _\n";
    }
    
    Expression *visitFunctionValue(FunctionValue* node) {
        error("UnsupportedFeatureError", "Functions are not yet supported in the SHARK architecture.");
        scope++;
        if (node->name == "ptr") {
            if (node->args[0]->is_numberConstant()) error("MemoryError", "Cannot take address of a number constant");
            if (node->args[0]->is_stringConstant()) error("MemoryError", "Cannot take address of a string constant");
            scope--;
            try {
                return new IntegerConstant(get<0>(variables[node->args[0]->constant()]));
            } catch (NotImplementedException e) {
                error("MemoryError", "Cannot take address of an expression");
            }
        } else {
            /*visitFunctionCall(new FunctionCall(
                node->name,
                node->args
            ));*/
            //return returned;
        }
        error("FunctionUndefinedError", "Function \"" + node->name + "\" is not defined");
        scope--;
        variableScope();
    }
    
    Expression *visitListConstant(ListConstant* node) {
        error("uhh", "hmm");
    }
    
    void variableScope() {
        unsigned int highest_addr = 0;
        std::vector<string> toErase{};
        for (auto itr = variables.begin(); itr != variables.end(); ++itr) {
            //cout << ((itr != variables.end()) ? "true" : "false") << endl;
            if (itr->first == "") continue;
            if (get<0>(itr->second) > highest_addr) highest_addr = get<0>(itr->second);
            if (get<1>(itr->second) > scope) {
                toErase.push_back(itr->first);
            }
        }
        for (string name: toErase) {
            variables.erase(name);
        }
        numvars = highest_addr;
    }
    
    void visit(Statement *node) {
        node->accept(this);
    }
    void visitVariableDecl(VariableDecl *node) {
        variables[node->name] = tuple<int, int> {numvars, scope};
        if (node->initialiser != nullptr) {
            this->visitExpression(node->initialiser);
            out += "pl a _ _\nst a " + to_string(numvars) + " _\n";
        }
        numvars++;
    }
    void visitAssignment(Assignment* node) {
        this->visitExpression(node->value);
        out += "pl a _ _\nst a " + to_string(get<0>(variables[node->name])) + " _\n";
    }
    void visitFunctionCall(FunctionCall* node) {
        if (node->name == "") return;
        if (node->name == "print") {
        scope++;
            for (Expression *arg : node->args) {
                std::cout << visitExpression(arg)->value() << std::endl;
            }
        } else {
            error("UnsupportedFeatureError", "Functions are not yet supported in the SHARK architecture.");
            if (functions.count(node->name) == 0) error("FunctionNotDefinedError", "Function \"" + node->name + "\" not defined.");
            for (int i = 0; i < node->args.size(); i++) {
                visitVariableDecl(
                    new VariableDecl(functions[node->name]->args[i]->name, functions[node->name]->args[i]->type, node->args[i])
                );
                //cout << i << " " << functions[node->name]->args[i]->name << endl;
            }
            visitCodeBlock(functions[node->name]->body);
        }
        scope--;
        variableScope();
    }
    void visitFunctionDecl(FunctionDecl *node) {
        error("UnsupportedFeatureError", "Functions are not yet supported in the SHARK architecture.");
    
        functions[node->name] = node;
    }
    void visitCodeBlock(CodeBlock* node) {
        for (Statement *statement : node->body) {
            visit(statement);
        }
    }
    void visitWhile(While* node) {
        error("", "WIP");
        while (visitExpression(node->cond)->value() != 0) {
            scope++;
            visitCodeBlock(node->body);
            scope--;
            variableScope();
        }
    }
    void visitIf(If* node) {
        error("", "WIP");
        if(visitExpression(node->cond)->value() != 0) {
            scope++;
            visitCodeBlock(node->body);
            scope--;
            variableScope();
        }
    }
    void visitReturn(Return* node) {
        error("UnsupportedFeatureError", "Functions are not yet supported in the SHARK architecture.");
       // returned = node->value;
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
#ifndef NOMAIN_CLASP_CPP
int main (int argc, char* argv[]) {
    if (argc < 2) error("usage", "clasp <filename> [-s]");

    string code = "{";
    code += load_file(argv[1]);
    code += '}';
    vector<Token> tokens;
    tokens = parse_tokens(code);
    ASTParser parser {tokens};
    CodeBlock *block = parser.codeblock();
    string shark_flag = "-s";
    if (argc > 2 && string(argv[2]) == shark_flag){
        SHARKCompiler compiler;
        compiler.visit(block);
        cout << endl << compiler.out << endl;
    } else {
        Interpreter interpreter;
        interpreter.visit(block);
    }
}

#endif
#endif 