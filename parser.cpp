#ifndef PARSER_CPP
#define PARSER_CPP

#define MAIN
#include "clasp_ast.cpp"
#include "lexer.cpp"
#include "get_type.cpp"
#include <vector>
#include <string>
#include <stdarg.h>
#include <typeinfo>
#include <cmath>

using namespace std;

class ASTParser {
    vector<Token> tokens;
    int current = 0;

    public:
        ASTParser (vector<Token> tokens) {
            this->tokens = tokens;
        }

        Token peek () {
            return tokens[current];
        }

        Token previous() {
            return tokens[current - 1];
        }

        bool isAtEnd() {
            return current == tokens.size() - 1;
        }

        Token advance() {
            if (!isAtEnd()) current++;
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

            if(peek().type == "IDENTIFIER") {
                // TODO: implement this
            }

            if(peek().type == "STRING") {
                return new StringConstant(advance().value);
            }

            if (matchName({"false"})) return new IntegerConstant(false);
            if (matchName({"true"})) return new IntegerConstant(true);
            if (matchName({"null"})) return new IntegerConstant(0);

            if (matchType({"NUMBER"})) {
                for (char c : previous().value)
                    if (c == '.') return new FixedConstant(stoi(previous().value) * 56636.0);
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
            Token tok0 = previous();
            Token tok1 = advance();
            if (tok0.type == "IDENTIFIER" && tok1.type == "OPERATOR") {
                return new Assignment(tok0.value, expression());
            } else if (tok0.type == "KEYWORD" && tok1.type == "IDENTIFIER") {
                if (tok0.value == "var") {
                    if (advance().value != ":") error ("SyntaxError", "exptected : after variable name");
                    string type = advance().value;
                    if (advance().value == "=")
                        return new VariableDecl(tok1.value, type, expression());
                    return new VariableDecl(tok1.value, type);
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
                        args.push_back(new VariableDecl(aName, type));
                    }
                    if (advance().value != ")") error ("SyntaxError", "exptected ) after argument list");
                    if (advance().value != ":") error ("SyntaxError", "exptected : after argument list");
                    string type = peek().value;
                    return new FunctionDecl(name, codeblock(), args, type);
                }
            }
            return new FunctionCall("",{});
        }

        CodeBlock *codeblock() {
            if (peek().value == "{") { // multiple lines
                int depth = 1;
                advance();
                Token current;
                vector<Statement *> statements;
                while (depth != 0) {
                    current = peek();
                    if (current.value == "{") depth++;
                    if (current.value == "}") depth--;
                    if (depth == 0) break;
                    if (isAtEnd()) error("SyntaxError", "Unexpected EOF while parsing");
                    statements.push_back(statement());
                }
                return new CodeBlock(statements);
            }
            advance();
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

    void visitExpression(Expression *node) {
        node->accept(this);
    }

    void visitBinaryExpression(BinaryExpression *node) {
        std::cout << "BinaryExpression (left=";
        this->visitExpression(node->left());
        std::cout << " op=" << node->op() << " right=";
        this->visitExpression(node->right());
        std::cout << ")";
    }

    void visitUnaryExpression(UnaryExpression *node) {
        std::cout << "UnaryExpression (op=" << node->op() << " right=";
        this->visitExpression(node->right());
        std::cout << ")";
    }

    void visitIntegerConstant(IntegerConstant *node) {
        std::cout << "IntegerConstant (value=" << node->value() << ")";
    }

    void visitFixedConstant(FixedConstant *node) {
        std::cout << "FixedConstant (value=" << (node->value() / 65536.0) << ")";
    }

    void visitStringConstant(StringConstant *node) {
        std::cout << "StringConstant (value=\"" << node->constant() << "\")";
    }
};

int main () {
    string expression = R"(
    {
        fn main (argv: list[int]): int {
            var a: int = 0;
            a = 1;
            print(a);
        }
    }
    )";

    vector<Token> tokens = parse_tokens(expression);

    ASTParser parser {tokens};

    ASTPrinter printer;

    printer.visitCodeBlock(
        parser.codeblock()
    );
}

#endif