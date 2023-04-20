#ifndef PARSER_CPP
#define PARSER_CPP

#define MAIN
#include "clasp_ast.cpp"
#include "lexer.cpp"
#include <vector>
#include <string>
#include <stdarg.h>

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
                    if (c == '.') return new FixedConstant(stoi(previous().value) * (2^32));
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
};

class ASTPrinter {
public:
  void print (Expression *expr) {
    std::cout << "(";
    try {
        try {
            print(expr->left());
        }catch (NotImplementedException e) {}
        std::cout << (expr->op());
        print(expr->right());
    }catch(NotImplementedException e){}
    try {
        std::cout << expr->value();
    }catch(NotImplementedException e){}
    try {
        std::cout << '"' << expr->constant() << '"';
    }catch(NotImplementedException e){}
    std::cout << ")";
  }
};

int main () {
    string expression = "5 + 6 * 2 ";

    vector<Token> tokens = parse_tokens(expression);
    print_tokens(tokens);

    ASTParser parser {tokens};

    Expression *expr = parser.expression();

    ASTPrinter printer {};
    printer.print(expr);
    cout << endl;
}

#endif