#ifndef PARSER_CPP
#define PARSER_CPP

#include "clasp_ast.cpp"
#include "lexer.cpp"
#include "get_type.cpp"
#include "ast_visitor.cpp"
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

        Token peek (int n = 0) {
            return tokens[current + n];
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
                return new Assignment(tok0.value, expression());
            } else if (tok0.type == "KEYWORD" && tok1.type == "IDENTIFIER") {
                if (tok0.value == "var") {
                    if (advance().value != ":") error ("SyntaxError", "exptected : after variable name");
                    string type = advance().value;
                    if (advance().value == "=")
                        return new VariableDecl(tok1.value, type, expression());
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
                while (previous().value != ")") {
                    args.push_back(expression());
                    if (peek().value != "," && peek().value != ")") error("SyntaxError", "exptected , after argument");
                    advance();
                }
                
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
            return new FunctionCall("",{});
        }

        CodeBlock *codeblock() {
            if (peek().value == "{") { // multiple lines
                int depth = 0;
                bool started = false;
                Token current;
                vector<Statement *> statements;
                cout << "START CODEBLOCK" << endl;
                while (depth != 0 || !started) {
                    current = peek();
                    cout << "CB:" << current.value << endl;
                    if (current.value == "{") depth++;
                    if (current.value == "}") depth--;
                    if (depth == 0 && started) break;
                    if (isAtEnd()) error("SyntaxError", "Unexpected EOF while parsing");
                    advance();
                    statements.push_back(statement());
                    started = true;
                }
                cout << "END CODEBLOCK" << endl;
                return new CodeBlock(statements);
            }
            //advance();
            return new CodeBlock({statement()});
        }
};

#ifndef MAIN
#define MAIN

int main () {
    string expression = R"(
    {
        fn main (argv: list[int]): int {
            var i: int = 0;
            while (i <= 10) 
                i = i + 1;
            
            print(
                "Hello world * ",
                i,
                "10"
            );
        }
    }
    )";

    vector<Token> tokens = parse_tokens(expression);

    ASTParser parser {tokens};
}

#endif
#endif