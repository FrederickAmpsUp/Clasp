#ifndef PARSER_CPP
#define PARSER_CPP

#define NOMAIN_PARSER_CPP // commenting this line allows the main function in this file to be created

#include "clasp_ast.cpp"
#include "lexer.cpp"
#include "get_type.cpp"
#include "ast_visitor.cpp"
#include <vector>
#include <string>
#include <stdarg.h>
#include <typeinfo>
#include <cmath>
#include <fstream>

using namespace std;

class ASTParser {
    vector<Token> tokens;
    int current = 0;

    public:
        ASTParser (vector<Token> tokens) {
            this->tokens = tokens;
        }

        Token peek (int n = 0) {
            return tokens[current + n];
        }

        Token previous() {
            return tokens[current - 1];
        }

        bool isAtEnd() {
            return current == tokens.size();// - 1;
        }

        Token advance(int n=1) {
            if (!isAtEnd()) current+=n;
            return previous();
        }


        bool checkName(string name) {
            if (isAtEnd()) return false;
            return peek().value == name;
        }

        bool checkType(string type) {
            if (isAtEnd()) return false;
            return peek().type == type;
        }

        bool matchName (vector<string> types) {
            for (string type : types) {
                if (checkName(type)) {
                    advance();
                    return true;
                }
            }
            return false;
        }

        bool matchType (vector<string> types) {
            for (string type : types) {
                if (checkType(type)) {
                    advance();
                    return true;
                }
            }
            return false;
        }

        Expression *primary() {
            if (peek().value == "[") {
                advance();
                vector<Expression *> values;
                while (peek().value != "]") {
                    values.push_back(expression());
                    
                    cout << advance().value << endl;
                }
            }
            if (peek().type == "IDENTIFIER" && peek(1).value == "(") {
                advance();
                string name = previous().value;
                vector<Expression*> args;
                while (advance().value != ")") {
                    args.push_back(expression());
                    if (peek().value != ")" && peek().value != ",") error("SyntaxError", "Expected ')' or ',' after argument");
                }
                return new FunctionValue(name, args);
            }
            if (matchName({"false"})) return new IntegerConstant(false);
            if (matchName({"true"})) return new IntegerConstant(true);
            if (matchName({"null"})) return new IntegerConstant(0);
            if(peek().type == "IDENTIFIER") {
                return new Variable(advance().value);
            }

            if(peek().type == "STRING") {
                return new StringConstant(advance().value);
            }

            if (matchType({"NUMBER"})) {
                for (char c : previous().value)
                    if (c == '.') return new FixedConstant(stoi(previous().value) * 65536.0);
                return new IntegerConstant(stoi(previous().value));
            }

            if (matchName({"("})) {
                Expression *expr = expression();
                if (peek().value != ")") 
                    error("SyntaxError", "Expected ')' after expression");
                advance();
                return expr;
            }
            error("SyntaxError", "Expected expression");
        }

        Expression *unary() {
            if (matchName({"!", "-"})) {
                Token op = previous();
                Expression *right = unary();
                return new UnaryExpression(op.value, right);
            }

            return primary();
        }
        
        Expression *factor() {
            Expression *expr = unary();

            while (matchName({"/", "*"})) {
                Token op = previous();
                Expression *right = unary();
                expr = new BinaryExpression(expr, op.value, right);
            }

            return expr;
        }

        Expression *term() {
            Expression *expr = factor();

            while (matchName({"-", "+"})) {
                Token op = previous();
                Expression *right = factor();
                expr = new BinaryExpression(expr, op.value, right);
            }

            return expr;
        }

        Expression *comparison () {
            Expression *expr = term();

            while (matchName({">", ">=", "<", "<="})) {
                Token op = previous();
                Expression *right = term();
                expr = new BinaryExpression(expr, op.value, right);
            }

            return expr;
        }

        Expression *equality () {
            Expression *expr = comparison();

            while(matchName({"!=", "=="})) {
                Token op = previous();
                Expression *right = comparison();
                expr = new BinaryExpression(expr, op.value, right);
            }
            return expr;
        }
        Expression *expression () {
            return equality();
        }

        Statement *statement () {
            if (peek().type == "DIRECTIVE") {
                // TODO: handle directives
            }

            Token tok0 = previous();
            Token tok1 = advance();
            
            //cout << tok0.value << " " << tok1.value << endl;
            if (tok0.type == "IDENTIFIER" && tok1.type == "OPERATOR") {
                Assignment *out = new Assignment(tok0.value, expression());
                if (advance().value != ";") error("SyntaxError", "Expected ';' after variable assignment");
                return out;
            } else if (tok0.type == "KEYWORD" && tok1.type == "IDENTIFIER") {
                if (tok0.value == "var") {
                    if (advance().value != ":") error ("SyntaxError", "exptected : after variable name");
                    string type = advance().value;
                    if (advance().value == "=") {
                        Expression *val = expression();
                        if (advance().value != ";") error ("SyntaxError", "expected ; after variable declaration");
                        return new VariableDecl(tok1.value, type, val);
                    }
                    if (advance().value != ";") error ("SyntaxError", "expected ; after variable declaration");
                    return new VariableDecl(tok1.value, type, nullptr);
                } if (tok0.value == "fn") {
                    string name = tok1.value;
                    advance();
                    vector<VariableDecl *> args;
                    while (peek().value != ")") {
                        string aName = advance().value;
                        if (advance().value != ":") error ("SyntaxError", "exptected : after argument name");
                        string type;
                        while (peek().value != "," && peek().value != ")") {
                            type += advance().value;
                        }
                        if (peek().value == ",")
                            advance();
                        args.push_back(new VariableDecl(aName, type));
                    }
                    if (advance().value != ")") error ("SyntaxError", "exptected ) after argument list");
                    if (advance().value != ":") error ("SyntaxError", "exptected : after argument list");
                    string type = advance().value;
                    return new FunctionDecl(name, codeblock(), args, type);
                }
            } else if (tok0.type == "IDENTIFIER" && tok1.value == "(") {
                string name = tok0.value;
                vector<Expression *> args;
                bool had_args = false;
                while (previous().value != ")" && peek().value != ")") {
                    args.push_back(expression());
                    if (peek().value != "," && peek().value != ")") error("SyntaxError", "exptected , after argument");
                    advance();
                    had_args = true;
                }
                if (had_args) advance(-1);
                if (advance().value != ")") error("SyntaxError", "exptected ) after function call args");
                if (advance().value != ";") error("SyntaxError", "exptected ; after function call");
                return new FunctionCall(name, args);
            } else if (tok0.type == "KEYWORD" && tok1.value == "(") {
                if (tok0.value == "while") {
                    Expression *cond = expression();
                    if (advance().value != ")") error("SyntaxError", "Expected ) after condition");
                    return new While(codeblock(), cond);
                } else if (tok0.value == "if") {
                    Expression *cond = expression();
                    if (advance().value != ")") error("SyntaxError", "Expected ) after condition");
                    return new If(codeblock(), cond);
                }
            } else if (tok0.value == "return") {
                current--;
                Expression *val = expression();
                if (advance().value != ";") error("SyntaxError", "Expected ; after return statement");
                return new Return(val);
            }
            advance(-1);
            return new FunctionCall("",{});
        }

        CodeBlock *codeblock() {
            if (peek().value == "{") { // multiple lines
                int depth = 1;
                bool started = false;
                Token current;
                vector<Statement *> statements;
                advance();
                while (depth != 0 || !started) {
                    current = advance();
                    if (current.value == "{") depth++;
                    if (current.value == "}") depth--;
                    if (depth == 0 && started) break;
                    //cout << current.value << " " << depth << endl;
                    if (isAtEnd()) error("SyntaxError", "Unexpected EOF while parsing code block");

                    statements.push_back(statement());
                    started = true;
                }
                return new CodeBlock(statements);
            }
            //advance();
            return new CodeBlock({statement()});
        }
};


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
    
    Expression *visitFunctionValue(FunctionValue *node) {
        std::cout << "FunctionValue(name=" << node->name << ", args=[";
        for (Expression *arg : node->args) {
            visitExpression(arg);
        }
        std::cout << "])";
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
    
    void visitReturn(Return *node) {
        std::cout << "Return (value=";
        visitExpression(node->value);
        std::cout << ")";
    }
};

#ifndef NOMAIN_PARSER_CPP

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

int main (int argc, char *argv[]) {
    if (argc < 2) error("usage", "clasp <filename>");
    string code = "{";
    code += load_file(argv[1]);
    code += '}';
    cout << code << endl;
    vector<Token> tokens;
    tokens = parse_tokens(code);
    ASTParser parser {tokens};
    ASTPrinter printer;
    printer.visit(parser.codeblock());
}

#endif
#endif